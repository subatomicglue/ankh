#!/bin/bash
set -euo pipefail

cd "$(dirname "$0")"

project_name=$(basename "$PWD")
project_slug=$(printf '%s' "$project_name" | LC_ALL=C tr -c 'A-Za-z0-9._-' '-' | cut -c 1-60)
server_mode=static
pm2_script=""
html_file="index.html"

if [ "${1:-}" = "--html" ]; then
  if [ -z "${2:-}" ]; then
    echo "Usage: $0 [--html filename.html | --pm2 script.js]"
    exit 1
  fi
  html_file="$2"
  shift 2
elif [ "${1:-}" = "--pm2" ]; then
  if [ -z "${2:-}" ]; then
    echo "Usage: $0 [--html filename.html | --pm2 script.js]"
    exit 1
  fi
  server_mode=pm2
  pm2_script="$2"
  shift 2
fi
if [ "$#" -ne 0 ]; then
  echo "Usage: $0 [--html filename.html | --pm2 script.js]"
  exit 1
fi
if [ "$server_mode" = static ] && [ ! -f "$html_file" ]; then
  echo "HTML file not found: $html_file"
  exit 1
fi

if ! command -v node >/dev/null 2>&1; then
  echo "Node.js is required to run the ANKH debug server."
  exit 1
fi

project_node=$(command -v node)
project_node_major=$($project_node -p "Number(process.versions.node.split('.')[0])")
for candidate in "${NVM_DIR:-$HOME/.nvm}"/versions/node/*/bin/node; do
  if [ ! -x "$candidate" ]; then continue; fi
  candidate_major=$($candidate -p "Number(process.versions.node.split('.')[0])")
  if [ "$candidate_major" -gt "$project_node_major" ]; then
    project_node="$candidate"
    project_node_major="$candidate_major"
  fi
done
if [ "$project_node_major" -lt 20 ]; then
  echo "Node.js 20 or newer is required to run the ANKH debug server."
  exit 1
fi

ip=$("$project_node" -e "const os=require('node:os'); const all=Object.values(os.networkInterfaces()).flat().filter(Boolean); const address=all.find(a=>a.family==='IPv4'&&!a.internal); process.stdout.write(address?.address??'')")
if [ -z "$ip" ]; then
  echo "Unable to find a LAN IPv4 address. Connect this Mac to the same network as the iPhone."
  exit 1
fi

requested_port="${DEBUG_SERVER_PORT:-${ANKH_DEBUG_SERVER_PORT:-}}"
if [ -n "$requested_port" ]; then
  port="$requested_port"
else
  port=$("$project_node" -e "const net=require('node:net'),s=net.createServer(); s.listen(0,'0.0.0',()=>{process.stdout.write(String(s.address().port));s.close()})")
fi
case "$port" in
  ''|*[!0-9]*) echo "Debug server port must be a number: $port"; exit 1 ;;
esac
if [ "$port" -lt 1 ] || [ "$port" -gt 65535 ]; then
  echo "Debug server port is out of range: $port"
  exit 1
fi

mkdir -p certs-debug
if [ ! -f certs-debug/ca-key.pem ] || [ ! -f certs-debug/ca.pem ]; then
  openssl genpkey -algorithm RSA -out certs-debug/ca-key.pem
  openssl req -x509 -new -sha256 -days 3650 -key certs-debug/ca-key.pem -out certs-debug/ca.pem -subj "/CN=$project_name Debug CA" -addext "basicConstraints=critical,CA:TRUE" -addext "keyUsage=critical,keyCertSign,cRLSign"
fi

if [ ! -f certs-debug/privkey.pem ] || [ ! -f certs-debug/cert.pem ] ||
   ! openssl x509 -checkend 86400 -noout -in certs-debug/cert.pem >/dev/null ||
   ! openssl x509 -noout -ext subjectAltName -in certs-debug/cert.pem | grep -q "IP Address:$ip"; then
  openssl genpkey -algorithm RSA -out certs-debug/privkey.pem
  openssl req -new -sha256 -key certs-debug/privkey.pem -out certs-debug/server.csr -subj "/CN=$ip" -addext "subjectAltName=IP:$ip,IP:127.0.0.1,DNS:localhost" -addext "extendedKeyUsage=serverAuth"
  openssl x509 -req -sha256 -days 30 -in certs-debug/server.csr -CA certs-debug/ca.pem -CAkey certs-debug/ca-key.pem -CAcreateserial -copy_extensions copy -out certs-debug/cert.pem
fi

echo ".::--------------------------------------------------::."
echo "Mac:    https://localhost:$port/"
echo "iPhone: https://$ip:$port/"
echo "Trust once: $(pwd)/certs-debug/ca.pem"
echo
echo "1. AirDrop certs-debug/ca.pem to the iPhone."
echo "2. Open Settings → General → VPN & Device Management → install the profile."
echo "3. Open Settings → General → About → Certificate Trust Settings."
echo "4. Enable full trust for “$project_name Debug CA.”"
echo ""
echo "If it doesn't work:"
echo "1. On iPhone, open Control Center."
echo "2. Press and hold the Wi-Fi/Bluetooth panel."
echo "3. Tap AirDrop."
echo "4. Select Everyone for 10 Minutes."
echo ".::--------------------------------------------------::."
echo

export DEBUG_SERVER_PORT="$port"
export DEBUG_CERTS_DIR="$(pwd)/certs-debug"
export DEBUG_LAN_URL="https://$ip:$port/"
export DEBUG_HTML_FILE="$html_file"
# Compatibility aliases for existing ANKH.js tooling.
export ANKH_DEBUG_SERVER_PORT="$DEBUG_SERVER_PORT"
export ANKH_DEBUG_CERTS_DIR="$DEBUG_CERTS_DIR"
export ANKH_DEBUG_LAN_URL="$DEBUG_LAN_URL"

run_server() {
  "$project_node" -e '
    const fs = require("node:fs");
    const https = require("node:https");
    const path = require("node:path");
    const root = process.cwd();
    const port = Number(process.env.DEBUG_SERVER_PORT);
    const certs = process.env.DEBUG_CERTS_DIR;
    const home = process.env.DEBUG_HTML_FILE;
    const types = {".bmp":"image/bmp",".css":"text/css; charset=utf-8",".html":"text/html; charset=utf-8",".js":"text/javascript; charset=utf-8",".json":"application/json; charset=utf-8",".m4a":"audio/mp4",".png":"image/png"};
    https.createServer({cert:fs.readFileSync(path.join(certs,"cert.pem")),key:fs.readFileSync(path.join(certs,"privkey.pem"))},(request,response)=>{
      try {
        const pathname = decodeURIComponent(new URL(request.url,"https://localhost").pathname);
        const filename = path.resolve(root,pathname === "/" ? home : pathname.replace(/^\/+/ ,""));
        if (filename !== root && !filename.startsWith(root + path.sep)) return response.writeHead(403).end("Forbidden");
        const stat = fs.statSync(filename);
        if (!stat.isFile()) throw new Error();
        response.writeHead(200,{"Cache-Control":"no-store","Content-Length":stat.size,"Content-Type":types[path.extname(filename).toLowerCase()] || "application/octet-stream"});
        request.method === "HEAD" ? response.end() : fs.createReadStream(filename).pipe(response);
      } catch { response.writeHead(404,{"Content-Type":"text/plain; charset=utf-8"}).end("Not found"); }
    }).listen(port,"0.0.0.0");
  '
}

run_pm2_server() {
  if ! command -v pm2-runtime >/dev/null 2>&1; then
    echo "pm2-runtime is required for --pm2 mode. Install PM2 first."
    return 1
  fi
  if [ ! -f "$pm2_script" ]; then
    echo "Node.js server not found: $pm2_script"
    return 1
  fi
  NODE_USE_HTTPS=true \
  NODE_HTTPS_PORT="$port" \
  NODE_CERTS_DIR="$DEBUG_CERTS_DIR" \
  pm2-runtime start "$pm2_script" --interpreter "$project_node" --name "$project_slug-debug"
}

run_selected_server() {
  if [ "$server_mode" = pm2 ]; then run_pm2_server; else run_server; fi
}

open_chrome="${DEBUG_OPEN_CHROME:-${ANKH_OPEN_CHROME:-false}}"
if [ "$open_chrome" = true ]; then
  run_selected_server &
  server_pid=$!
  trap 'kill "$server_pid" 2>/dev/null || true' EXIT INT TERM
  server_ready=false
  for _ in {1..50}; do
    if ! kill -0 "$server_pid" 2>/dev/null; then
      wait "$server_pid"
      exit $?
    fi
    if curl --silent --insecure --head "https://localhost:$port/" >/dev/null 2>&1; then
      server_ready=true
      break
    fi
    sleep 0.1
  done
  if [ "$server_ready" != true ]; then
    echo "Debug server did not become ready."
    exit 1
  fi
  DEBUG_CHROME_START_URL="${DEBUG_CHROME_START_URL:-${ANKH_CHROME_START_URL:-https://localhost:$port/}}" ./start-debug-chrome.sh
  wait "$server_pid"
else
  run_selected_server
fi
