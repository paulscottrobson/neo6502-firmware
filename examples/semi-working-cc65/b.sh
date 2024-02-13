rm *.o main
ca65  crt0.s
ar65 a neo6502.lib crt0.o
cc65 --static-locals -t none -O --cpu 65c02 main.c
ca65 --cpu 65c02 main.s -o main.o
ld65 -C neo6502.cfg --lib neo6502.lib --lib none.lib -o main main.o
../../bin/neo main@800 cold

