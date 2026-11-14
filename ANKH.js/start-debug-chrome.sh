#!/bin/bash
set -euo pipefail

chrome_app="/Applications/Google Chrome.app"
chrome_debug_port="${DEBUG_CHROME_PORT:-${ANKH_CHROME_DEBUG_PORT:-9222}}"
chrome_profile_input="${DEBUG_CHROME_PROFILE:-${ANKH_CHROME_PROFILE_DIR:-debug-chrome-profile}}"
chrome_start_url="${DEBUG_CHROME_START_URL:-${ANKH_CHROME_START_URL:-https://localhost:${DEBUG_SERVER_PORT:-${ANKH_DEBUG_SERVER_PORT:-3003}}/}}"

# DEBUG_CHROME_PROFILE is intentionally treated as a name, not a path.
# Replace separators and other unsafe characters before placing it under /tmp.
chrome_profile_name=$(printf '%s' "$chrome_profile_input" | LC_ALL=C tr -c 'A-Za-z0-9._-' '_' | cut -c 1-80)
if [ -z "$chrome_profile_name" ]; then
  chrome_profile_name="debug-chrome-profile"
fi
if [ "$chrome_profile_name" = "." ] || [ "$chrome_profile_name" = ".." ]; then
  chrome_profile_name="_$chrome_profile_name"
fi
chrome_profile_dir="/tmp/$chrome_profile_name"

if [ ! -d "$chrome_app" ]; then
  echo "Google Chrome was not found at: $chrome_app"
  exit 1
fi

if [ "$(uname)" != "Darwin" ]; then
  echo "start-debug-chrome.sh currently supports macOS only."
  exit 1
fi

if [ -L "$chrome_profile_dir" ]; then
  echo "Refusing to use a symbolic link as the Chrome profile: $chrome_profile_dir"
  exit 1
fi
mkdir -p "$chrome_profile_dir"

echo ""
echo "Launching an isolated Chrome profile"
echo "  profile: $chrome_profile_dir   <-- temporary Chrome user-data folder"
echo "  CDP:     http://127.0.0.1:$chrome_debug_port  <-- Chrome DevTools Protocol"
echo
echo "This profile does not use cookies, history, passwords, extensions, or sync"
echo "from your normal Chrome profile. Close its Chrome window when finished."

open -na "Google Chrome" --args \
  --user-data-dir="$chrome_profile_dir" \
  --remote-debugging-address=127.0.0.1 \
  --remote-debugging-port="$chrome_debug_port" \
  --ignore-certificate-errors \
  --no-first-run \
  --no-default-browser-check \
  "$chrome_start_url"

