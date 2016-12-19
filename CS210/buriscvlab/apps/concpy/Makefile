include lib/common.mk

.PHONY: all

all: concpy512K-64.img concpy1M-64.img

concpy-64.o: concpy.S
	$(CC) $(CFLAGS64) -c concpy.S -o concpy-64.o

concpy512K-64.elf: ${LDS512K64} concpy-64.o
	$(CC) ${CFLAGS64} -nostartfiles -T ${LDS512K64} ${RESET64} -o concpy512K-64.elf concpy-64.o > concpy512K-64.map

concpy512K-64.img: concpy512K-64.elf
	$(OBJCOPY) -S concpy512K-64.elf --output-target=binary concpy512K-64.img

concpy1M-64.elf: ${LDS1M64} concpy-64.o
	$(CC) ${CFLAGS64} -nostartfiles -T ${LDS1M64} ${RESET64} -o concpy1M-64.elf concpy-64.o > concpy1M-64.map

concpy1M-64.img: concpy1M-64.elf
	$(OBJCOPY) -S concpy1M-64.elf --output-target=binary concpy1M-64.img

clean:
	-rm -rf $(wildcard *.elf *.o *.img *.map)
