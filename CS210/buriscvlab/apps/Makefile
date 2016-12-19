BURISCVDIR=/research/sesa/210/buriscv

.PHONY: all

all:
	make -I $(BURISCVDIR) -C memcpy
	make -I $(BURISCVDIR) -C concpy
	make -I $(BURISCVDIR) -C jalret
	make -I $(BURISCVDIR) -C func
	make -I $(BURISCVDIR) -C printf
	make -I $(BURISCVDIR) -C ptypes

clean:
	make -I $(BURISCVDIR) -C memcpy clean
	make -I $(BURISCVDIR) -C concpy clean
	make -I $(BURISCVDIR) -C jalret clean
	make -I $(BURISCVDIR) -C func clean
	make -I $(BURISCVDIR) -C printf clean
	make -I $(BURISCVDIR) -C ptypes clean

