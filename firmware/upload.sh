make -C build
cp build/sources/firmware.elf .
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program firmware.elf verify reset exit"
