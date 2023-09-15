#----------------------------------------------------------------------------
CC=avr-gcc
OBJCOPY=avr-objcopy

all:
	${CC} ${CFLAGS24} -o ${TARGET24}.bin ${SRCS24}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET24}.bin ${TARGET24}.hex
	${CC} ${CFLAGS8} -o ${TARGET8}.bin ${SRCS8}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET8}.bin ${TARGET8}.hex

clean:
	rm -f *.bin *.hex

#----------------------------------------------------------------------------

MCU24=attiny24
F_CPU24=1200000
CFLAGS24=-std=c99 -Wall -g -Os -mmcu=${MCU24} -DF_CPU=${F_CPU24} -I.
TARGET24=main_7seg
SRCS24=main_7seg.c

all24:
	${CC} ${CFLAGS24} -o ${TARGET24}.bin ${SRCS24}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET24}.bin ${TARGET24}.hex

#----------------------------------------------------------------------------

MCU8=atmega8
F_CPU8=8000000UL
CFLAGS8=-std=c99 -Wall -g -Os -mmcu=${MCU8} -DF_CPU=${F_CPU8} -I.
TARGET8=main
SRCS8=main.c

all8:
	${CC} ${CFLAGS8} -o ${TARGET8}.bin ${SRCS8}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET8}.bin ${TARGET8}.hex

#----------------------------------------------------------------------------