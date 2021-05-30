export base=${1%.*}
as -g "$base.S" -o "$base.o"
objcopy -O binary -j .text "$base.o" "$base.bin"
rm "$base.o"
hexdump -v -e '"\\""x" 1/1 "%02x" ""' "$base.bin" > "$base.txt"