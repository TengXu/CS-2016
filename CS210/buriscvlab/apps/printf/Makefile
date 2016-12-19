include lib/common.mk

.PHONY: all

all: printf512K-64.img printf1M-64.img

printf-64.o: printf.c
	$(CC) $(CFLAGS64) -c printf.c -o printf-64.o

printf512K-64.elf: ${LDS512K64} ${CRT064} printf-64.o
	$(CC) ${CFLAGS64} -T ${LDS512K64} -o printf512K-64.elf ${RESET64} printf-64.o ${LIBS64} ${SYSLIB64} ${LIBS64} > printf512K-64.map

printf512K-64.img: printf512K-64.elf
	$(OBJCOPY) -S printf512K-64.elf --output-target=binary printf512K-64.img

printf1M-64.elf: ${LDS1M64} ${CRT064} printf-64.o
	$(CC) ${CFLAGS64} -T ${LDS1M64} -o printf1M-64.elf ${RESET64} printf-64.o ${LIBS64} ${SYSLIB64} ${LIBS64} > printf1M-64.map

printf1M-64.img: printf1M-64.elf
	$(OBJCOPY) -S printf1M-64.elf --output-target=binary printf1M-64.img
clean:
	-rm -rf $(wildcard *.elf *.o *.img *.map)
