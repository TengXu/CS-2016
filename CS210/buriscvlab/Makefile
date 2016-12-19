# Copyright (C) 2011 by Jonathan Appavoo, Boston University
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
CC=gcc
O=1
DEBUG=-g

CFLAGS=-O${O} -Wall ${DEBUG} -D IMP
COMPILE.c = $(CC) $(CFLAGS) -c
LINK.o = $(CC) $(LDFLAGS) 
OUTPUT_OPTION=-MMD -MP -o $@

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  STAT=gstat
else
  STAT=stat
endif

%.o64: %.c
	$(COMPILE.c) -DXLEN64 $(OUTPUT_OPTION) $<

SRC=$(wildcard *.c)
OBJ64=$(SRC:.c=.o64)
DEP=$(SRC:.c=.d)

.PHONY: clean all apps required bonus bonus1 bonus2 bonus3 bonus4 full

all: buriscv64

buriscv64: $(OBJ64)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

-include $(DEP)

full: required bonus
required: memcpy64 concpy64 jalret64 func64 
bonus: bonus1 bonus2 bonus3 bonus4
bonus1: printf64 
bonus2: ptypes64 
bonus3: unknown1-64 
bonus4: unknown2-64

memcpy64: buriscv64 apps
	bash -c './buriscv64 apps/memcpy/memcpy512K-64.img memcpy-64.img.out < /dev/null  > /dev/null  2> memcpy-64.trc'
	@echo "INPUT:" > memcpy-64.res
	@./img dump apps/memcpy/memcpy512K-64.img >> memcpy-64.res
	@echo "OUTPUT:" >> memcpy-64.res
	@./img dump memcpy-64.img.out >> memcpy-64.res
	@bash -c 'num=$$(grep ello memcpy-64.res | wc -l);\
	 if (( num == 3 )); then echo MEMCOPY64: PASS; else echo MEMCOPY64: FAIL; fi' 

concpy64: buriscv64 apps
	bash -c 'echo "Hello World" | ./buriscv64 apps/concpy/concpy512K-64.img concpy-64.img.out  > concpy-64.con  2> concpy-64.trc'
	@cat concpy-64.con
	@bash -c 'res=$$(cat concpy-64.con);\
	 if [[ $$res = "Hello World" ]]; then echo CONCPY64: PASS; else echo CONCPY64: FAIL; fi' 

jalret64: buriscv64 apps
	bash -c './buriscv64 apps/jalret/jalret512K-64.img jalret-64.img.out < /dev/null  > /dev/null  2> jalret-64.trc'
	@echo "INPUT:" > jalret-64.res
	@./img dump apps/jalret/jalret512K-64.img >> jalret-64.res
	@echo "OUTPUT:" >> jalret-64.res
	@./img dump jalret-64.img.out >> jalret-64.res
	@bash -c 'num=$$(grep Lump jalret-64.res | wc -l);\
	 if (( num == 1 )); then echo JALRET64: PASS; else echo JALRET64: FAIL; fi' 

func64: buriscv64 apps
	bash -c './buriscv64 apps/func/func512K-64.img func-64.img.out < /dev/null  > /dev/null  2> func-64.trc'
	@echo "INPUT:" > func-64.res
	@./img dump apps/func/func512K-64.img >> func-64.res
	@echo "OUTPUT:" >> func-64.res
	@./img dump func-64.img.out >> func-64.res
	@bash -c 'num1=$$(grep "func1" func-64.res | wc -l);\
         num2=$$(grep "00010210  1a 00 00 00 0d 00 00 00  00 00 00 00 00 00 00 00" func-64.res | wc -l);\
	 if (( num1 == 3 )) && (( num2 == 1 )); then echo FUNC64: PASS; else echo FUNC64: FAIL; fi' 

printf64: buriscv64 apps
	bash -c './buriscv64 apps/printf/printf512K-64.img printf-64.img.out > printf-64.con 2> printf-64.trc'
	cat printf-64.con
	@bash -c 'res=$$(cat printf-64.con);size=$$($(STAT) -c %s printf-64.con);\
	 if [[ $$res = "Hello World!!!" && $$size = 15 ]]; then echo PRINTF64: PASS; else echo PRINTF64: FAIL; fi' 

ptypes64: buriscv64 apps
	bash -c './buriscv64 apps/ptypes/ptypes512K-64.img ptypes-64.img.out > ptypes-64.con 2> ptypes-64.trc'
	cat ptypes-64.con
	@bash -c 'res=$$(cat ptypes-64.con);size=$$($(STAT) -c %s ptypes-64.con);\
	 if [[ $$res = "c: -1 < 1 uc: 255 > 1 s: -1 < 1 us: 65535 > 1 i: -1 < 1 ui: 4294967295 > 1 ll: -1 < 1 ull: 18446744073709551615 > 1 c=-1 (0xffff) s=-1 (0xffff) i=-1 (0xffffffff) l=-1 (0xffffffffffffffff) ll=-1 (0xffffffffffffffff)" && $$size = 215 ]]; then echo PTYPES64: PASS; else echo PTYPES64: FAIL; fi' 

unknown1-64: buriscv64 apps
	bash -c 'echo -e "q" | ./buriscv64 apps/unknown/unknown512K-64.img unknown512K-64.img.out > unknown1-64.con 2> unknown1-64.trc'
	@bash -c 'num=$$(grep 0 unknown1-64.con | wc -l);\
	 if (( num == 12 )); then echo UNKNOWN1: PASS; else echo UNKNOWN1: FAIL; fi' 

unknown2-64: buriscv64 apps
	bash -c 'echo -e "cp6343\np7655\np7245\np7152\np4334\np4536\np5233\np0002\n3625\np7362\np6444\np7531\np5543\np4324\np3325\np2513\np7072\np2412\np6271\np7170\np7473\np7323\np3122\np2322\npq" | ./buriscv64 apps/unknown/unknown512K-64.img unknown512K-64.img.out > unknown2-64.con 2> unknown2-64.trc'
	@bash -c 'num1=$$(grep FF.FF.FF unknown2-64.con | wc -l);\
	if (( num1 == 1 )); then echo UNKNOWN2: PASS; else echo UNKNOWN2: FAIL; fi'

apps:
	make -C apps

compile: 
	@bash -c 'make clean; make 2> compile.out; num=$$(wc -l compile.out); info=$$(./buriscv64 2>&1 | grep RISCVLab);\
	 team=$${info#*teamname=};team=$${team%% m0=*}; echo info: $${info};\
	 if [[ -a buriscv64 && $$num=0 && -n $$team ]] ; then echo COMPILE: PASS; else echo COMPILE: FAIL -- BE SURE TO UPDATE team.c; fi' 

clean:
	-rm -rf $(wildcard $(OBJ64) $(DEP) buriscv32 buriscv64 handout cswitch *.res *.con *.out *.trc *.img *.dumpmake handout handout.tgz handout.md5sum  handout.tar handout.tar.gz)
