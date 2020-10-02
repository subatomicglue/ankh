

for filepath in src/*.bmp; do
  filename=$(basename -s ".bmp" "$filepath")
  echo "convert $filepath -transparent "#000000" ./${filename}.png"
  convert $filepath -transparent "#000000" ./${filename}.png
done



