/******************************************************************************
* Copyright (C) 2011 by Jonathan Appavoo, Boston University
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "common.h"

void dump_inst(Instruction i) { fprintf(stderr, "0x%08x : ...\n", i.raw); }

void dump_gprs(struct RegisterFile* rf) {
  int i = 0;
  for (i = 0; i < NUM_GPRS; i++) {
    fprintf(stderr, "%s(%s):" PRI_REG " ", gprsNames[i].gprName,
            gprsNames[i].ccName, rf->gprs.reg[i]);
    if ((i + 1) % 4 == 0) fprintf(stderr, "\n");
  }
  if ((i + 1) % 4 != 0) fprintf(stderr, "\n");
}

void dump_mcpuid(MCpuid r) { fprintf(stderr, "mcpuid:" PRI_REG, r.raw); }

void dump_mimpid(MImpid r) { fprintf(stderr, "mimpid:" PRI_REG, r.raw); }

void dump_hart(Register r) { fprintf(stderr, "hart:" PRI_REG, r); }

void dump_mstatus(MStatus r) { fprintf(stderr, "mstatus:" PRI_REG, r.raw); }

void dump_mtvec(Register r) { fprintf(stderr, "mtvec:" PRI_REG, r); }

void dump_mtdeleg(Register r) { fprintf(stderr, "mtdeleg:" PRI_REG, r); }

void dump_mip(MIp r) { fprintf(stderr, "mip:" PRI_REG, r.raw); }

void dump_mie(MIe r) { fprintf(stderr, "mie:" PRI_REG, r.raw); }

void dump_mtime(Register r) { fprintf(stderr, "mtime:" PRI_REG, r); }

void dump_mtimecmp(Register r) { fprintf(stderr, "mtimecmp:" PRI_REG, r); }

void dump_mscratch(Register r) { fprintf(stderr, "mscratch:" PRI_REG, r); }

void dump_mepc(Register r) { fprintf(stderr, "mepc:" PRI_REG, r); }

void dump_mcause(MCause r) { fprintf(stderr, "dump_mcause:" PRI_REG, r.raw); }

void dump_csrs(struct RegisterFile* rf) {
  fprintf(stderr, "csrs: machine:\n");
  dump_mcpuid(rf->csrs.machine.mcpuid);
  fprintf(stderr, " ");
  dump_mimpid(rf->csrs.machine.mimpid);
  fprintf(stderr, " ");
  dump_hart(rf->csrs.machine.mhartid);
  fprintf(stderr, " ");
  dump_mstatus(rf->csrs.machine.mstatus);
  fprintf(stderr, " ");
  dump_mtvec(rf->csrs.machine.mtvec);
  fprintf(stderr, " ");
  dump_mtdeleg(rf->csrs.machine.mtdeleg);
  fprintf(stderr, "\n");
  dump_mip(rf->csrs.machine.mip);
  fprintf(stderr, " ");
  dump_mie(rf->csrs.machine.mie);
  fprintf(stderr, " ");
  dump_mtime(rf->csrs.machine.mtime);
  fprintf(stderr, " ");
  dump_mtimecmp(rf->csrs.machine.mtimecmp);
  fprintf(stderr, " ");
  dump_mscratch(rf->csrs.machine.mscratch);
  fprintf(stderr, "\n");
  dump_mepc(rf->csrs.machine.mepc);
  fprintf(stderr, " ");
  dump_mcause(rf->csrs.machine.mcause);
  fprintf(stderr, " ");
}

void dump_regfile(struct RegisterFile* rf) {
  dump_gprs(rf);
  dump_csrs(rf);
}

void dump_cpu(struct Machine* m) {
  dump_regfile(&(m->state.cpu.core.regFile));
  fprintf(stderr, "ir:");
  dump_inst(m->state.cpu.core.ir);
}

void dump_vectors(struct Machine* m) {
  fprintf(stderr, "vectors: USR:" PRI_ADDR " SUP:" PRI_ADDR " HYP:" PRI_ADDR
                  " MAC:" PRI_ADDR " NMI:" PRI_ADDR " RST:" PRI_ADDR "\n",
          int_getVector(m, USER), int_getVector(m, SUPERVISOR),
          int_getVector(m, HYPERVISOR), int_getVector(m, MACHINE),
          int_getVector(m, NMI), int_getVector(m, RESET));
}

void dump_mem(struct Machine* m, Address addr, Word len) {
  Word i;
  for (i = 0; i < len; i++) {
    if (addr + i >= m->info.memSize) return;
    if (i % 16 == 0) {
      if (i) fprintf(stderr, "\n");
      fprintf(stderr, PRI_ADDR ":", addr + i);
    }
    fprintf(stderr, " %02x", m->state.memory[addr + i]);
  }
  if ((i - 1) % 16 != 0) fprintf(stderr, "\n");
}


void dump_machine(struct Machine* m, enum DumpMemDirective md) {
  fprintf(stderr, "memSize:" PRI_ADDR "\n", m->info.memSize);
  fprintf(stderr, "ioBaseAddr:" PRI_ADDR "\n", m->info.ioBaseAddr);
  fprintf(stderr, "ioLen:" PRI_NUM "\n", m->info.ioLen);

  dump_cpu(m);
  dump_vectors(m);

  if (md == MEM_ALL)
    dump_mem(m, 0, m->info.memSize);
  else if (md == MEM_SPECIAL) {
    // dump vectors and a little bit after
    fprintf(stderr, "BURiscv Info Structure:\n");
    fprintf(stderr, "Trap vectors:\n");
    dump_mem(m, TRAP_VECTOR_BASE, RESET + 4);
    fprintf(stderr, "Other:\n");
    dump_mem(m, TRAP_VECTOR_BASE + RESET + 4, 32);
  }
}

void dump_nextInstructionState(struct Machine* m) {
  dump_gprs(&(m->state.cpu.core.regFile));
  {
    fprintf(stderr, "%llu ", m->info.instCount);
    Register thePC = m->state.cpu.core.regFile.gprs.reg[pc];
    disassemble(thePC, (uint32_t*)&(m->state.memory[thePC]), 1);
  }
}
