include lib/common.mk

.PHONY: all

all: memcpy512K-64.img memcpy1M-64.img

memcpy-64.o: memcpy.S
	$(CC) $(CFLAGS64) -c memcpy.S -o memcpy-64.o

memcpy512K-64.elf: ${LDS512K64} memcpy-64.o
	$(CC) ${CFLAGS64} -nostartfiles -T ${LDS512K64} ${RESET32} -o memcpy512K-64.elf memcpy-64.o > memcpy512K-64.map

memcpy512K-64.img: memcpy512K-64.elf
	$(OBJCOPY) -S memcpy512K-64.elf --output-target=binary memcpy512K-64.img

memcpy1M-64.elf: ${LDS1M64}  memcpy-64.o
	$(CC) ${CFLAGS} -nostartfiles -T ${LDS1M64} ${RESET32} -o memcpy1M-64.elf memcpy-64.o > memcpy1M-64.map

memcpy1M-64.img: memcpy1M-64.elf
	$(OBJCOPY) -S memcpy1M-64.elf --output-target=binary memcpy1M-64.img

clean:
	-rm -rf $(wildcard *.elf *.o *.img *.map)
