set -e
rm -f sources/binary.h
64tass -c -b -Wall kernel/test.asm -o kernel/test.bin -L kernel/test.lst
python kernel/hconvert.py kernel/test.bin >sources/binary.h
touch sources/roms.*
make -C build
cp build/sources/firmware.elf .
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program firmware.elf verify reset exit"
