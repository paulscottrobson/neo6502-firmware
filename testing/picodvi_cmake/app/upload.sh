cp build/src/hello_dvi.elf start.elf
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program start.elf verify reset exit"
