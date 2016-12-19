include lib/common.mk

.PHONY: all

all: ptypes512K-64.img ptypes1M-64.img

ptypes64.o: ptypes.c
	$(CC) $(CFLAGS64) -c ptypes.c -o ptypes64.o


ptypes512K-64.elf: ${LDS512K64} ${RESET64} ptypes64.o
	$(CC) ${CFLAGS64} -T ${LDS512K64} -o ptypes512K-64.elf ${RESET64} ptypes64.o  > ptypes512K-64.map

ptypes512K-64.img: ptypes512K-64.elf
	$(OBJCOPY) -S ptypes512K-64.elf --output-target=binary ptypes512K-64.img

ptypes1M-64.elf: ${LDS1M64} ${RESET64} ptypes64.o
	$(CC) ${CFLAGS64} -T ${LDS1M64} -o ptypes1M-64.elf ${RESET64} ptypes64.o > ptypes1M-64.map

ptypes1M-64.img: ptypes1M-64.elf
	$(OBJCOPY) -S ptypes1M-64.elf --output-target=binary ptypes1M-64.img

clean:
	-rm -rf $(wildcard *.elf *.o *.img *.map)
