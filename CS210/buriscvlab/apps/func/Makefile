include lib/common.mk

.PHONY: all

all: func512K-64.img func1M-64.img
CFLAGS64+=-nostartfiles 
CRT=crt0.S

func-64.o: func.c
	$(CC) $(CFLAGS64) -c func.c -o func-64.o

func512K-64.elf: ${LDS512K64} crt0.S func-64.o
	$(CC) ${CFLAGS64} -T ${LDS512K64} -o func512K-64.elf ${CRT} ${RESET64} func-64.o ${LIBS64} > func512K-64.map

func512K-64.img: func512K-64.elf
	$(OBJCOPY) -S func512K-64.elf --output-target=binary func512K-64.img

func1M-64.elf: ${LDS1M64} crt0.S func-64.o
	$(CC) ${CFLAGS64} -T ${LDS1M64} -o func1M-64.elf ${CRT} ${RESET64} func-64.o ${LIBS64} > func1M-64.map

func1M-64.img: func1M-64.elf
	$(OBJCOPY) -S func1M-64.elf --output-target=binary func1M-64.img

clean:
	-rm -rf $(wildcard *.elf *.o *.img *.map)
