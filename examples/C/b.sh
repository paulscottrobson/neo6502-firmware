rm *.o
ca65  crt0.s
ar65 a neo6502.lib crt0.o
cc65 --static-locals -t none -O --cpu 65c02 main.c
ca65 --cpu 65c02 main.s
ld65 neo6502.cfg -o main -m main.map main.o neo6502.lib

../../bin/neo main@800 cold
