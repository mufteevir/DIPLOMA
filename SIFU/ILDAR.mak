CC = iccavr
LIB = ilibw
CFLAGS =  -e -D__ICC_VERSION=722 -DATMega32  -l -g -MLongJump -MHasMul -MEnhanced 
ASFLAGS = $(CFLAGS) 
LFLAGS =  -g -e:0x8000 -ucrtatmega.o -bfunc_lit:0x54.0x8000 -dram_end:0x85f -bdata:0x60.0x85f -dhwstk_size:30 -beeprom:0.1024 -fihx_coff -S2
FILES = ildar.o 

ILDAR:	$(FILES)
	$(CC) -o ILDAR $(LFLAGS) @ILDAR.lk   -lcatmega
ildar.o: C:\iccv7avr\include\iom32v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h
ildar.o:	..\COPYSI~1\ildar.c
	$(CC) -c $(CFLAGS) ..\COPYSI~1\ildar.c
