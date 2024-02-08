rm *.o
ca65  crt0.s
ar65 a neo6502.lib crt0.o

cl65 --static-locals -C neo6502.cfg -O --cpu 65c02 -o main2 -m main.map main.c neo6502.lib

../bin/neo main2@800 cold

