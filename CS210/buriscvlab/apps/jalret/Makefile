include lib/common.mk

.PHONY: all

all: jalret512K-64.img jalret1M-64.img

jalret-64.o: jalret.S
	$(CC) $(CFLAGS64) -c jalret.S -o jalret-64.o

jalret512K-64.elf: ${LDS512K64} jalret-64.o
	$(CC) ${CFLAGS64} -nostartfiles -T ${LDS512K64} ${RESET32} -o jalret512K-64.elf jalret-64.o > jalret512K-64.map

jalret512K-64.img: jalret512K-64.elf
	$(OBJCOPY) -S jalret512K-64.elf --output-target=binary jalret512K-64.img

jalret1M-64.elf: ${LDS1M64}  jalret-64.o
	$(CC) ${CFLAGS} -nostartfiles -T ${LDS1M64} ${RESET32} -o jalret1M-64.elf jalret-64.o > jalret1M-64.map

jalret1M-64.img: jalret1M-64.elf
	$(OBJCOPY) -S jalret1M-64.elf --output-target=binary jalret1M-64.img

clean:
	-rm -rf $(wildcard *.elf *.o *.img *.map)
