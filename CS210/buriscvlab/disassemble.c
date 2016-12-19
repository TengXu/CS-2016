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

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "common.h"

static int diss_LUI(Register addr, Instruction ir) {
  uint8_t rd = ir.U.rd;
  Register imm = decodeImmediate(ir, U);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tLUI \t%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rd].ccName, imm);

  return 1;
}

static int diss_AUIPC(Register addr, Instruction ir) {
  uint8_t rd = ir.U.rd;
  Register imm = decodeImmediate(ir, U);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tAUIPC \t%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rd].ccName, imm);
  return 1;
}

static int diss_JAL(Register addr, Instruction ir) {
  uint8_t rd = ir.J.rd;
  Register imm = decodeImmediate(ir, J);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tJAL \t%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rd].ccName, imm);
  return 1;
}

static int diss_JALR(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tJALR \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_BEQ(Register addr, Instruction ir) {
  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tBEQ \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_BNE(Register addr, Instruction ir) {
  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tBNE \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_BLT(Register addr, Instruction ir) {
  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tBLT \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_BGE(Register addr, Instruction ir) {
  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tBGE \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_BLTU(Register addr, Instruction ir) {
  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tBLTU \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_BGEU(Register addr, Instruction ir) {
  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tBGEU \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_LB(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tLB \t%s,%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_LH(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tLH \t%s,%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_LW(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tLW \t%s,%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_LBU(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tLBU \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_LHU(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tLHU \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_SB(Register addr, Instruction ir) {
  uint8_t rs1 = ir.S.rs1;
  uint8_t rs2 = ir.S.rs2;
  Register imm = decodeImmediate(ir, S);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSB \t%s,%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_SH(Register addr, Instruction ir) {
  uint8_t rs1 = ir.S.rs1;
  uint8_t rs2 = ir.S.rs2;
  Register imm = decodeImmediate(ir, S);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSH \t%s,%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_SW(Register addr, Instruction ir) {
  uint8_t rs1 = ir.S.rs1;
  uint8_t rs2 = ir.S.rs2;
  Register imm = decodeImmediate(ir, S);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSW \t%s,%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_ADDI(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tADDI \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_SLTI(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSLTI \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_SLTIU(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSLTIU \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_XORI(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tXORI \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_ORI(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tORI \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_ANDI(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tANDI \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_ADD(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tADD \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SUB(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSUB \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SLL(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSLL \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SLT(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSLT \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SLTU(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSLTU \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_XOR(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tXOR \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SRL(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSRL \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SRA(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSRA \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_OR(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tOR \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_AND(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tAND \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_FENCE(Register addr, Instruction ir) {
  fprintf(stderr, PRI_ADDR " (0x%08x) \tFENCE \t\n", addr, ir.raw);
  return 1;
}

static int diss_FENCEI(Register addr, Instruction ir) {
  fprintf(stderr, PRI_ADDR " (0x%08x) \tFENCEI \t\n", addr, ir.raw);
  return 1;
}

static int diss_SCALL(Register addr, Instruction ir) {
  fprintf(stderr, PRI_ADDR " (0x%08x) \tSCALL \t\n", addr, ir.raw);
  return 1;
}

static int diss_SBREAK(Register addr, Instruction ir) {
  fprintf(stderr, PRI_ADDR " (0x%08x) \tSBREAK \t\n", addr, ir.raw);
  return 1;
}

static int diss_RDCYCLE(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tRDCYCLE \t%s\n", addr, ir.raw,
          gprsNames[rd].ccName);
  return 1;
}

static int diss_RDCYCLEH(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tRDCYCLEH \t%s\n", addr, ir.raw,
          gprsNames[rd].ccName);
  return 1;
}

static int diss_RDTIME(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tRDTIME \t%s\n", addr, ir.raw,
          gprsNames[rd].ccName);
  return 1;
}

static int diss_RDTIMEH(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tRDTIMEH \t%s\n", addr, ir.raw,
          gprsNames[rd].ccName);
  return 1;
}

static int diss_RDINSTRET(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tRDINSTRET \t%s\n", addr, ir.raw,
          gprsNames[rd].ccName);
  return 1;
}

static int diss_RDINSTRETH(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tRDINSTRETH \t%s\n", addr, ir.raw,
          gprsNames[rd].ccName);
  return 1;
}

// RV64I
static int diss_LWU(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tLWU \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_LD(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tLD \t%s,%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_SD(Register addr, Instruction ir) {
  uint8_t rs1 = ir.S.rs1;
  uint8_t rs2 = ir.S.rs2;
  Register imm = decodeImmediate(ir, S);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSD \t%s,%s," PRI_REG "\n", addr, ir.raw,
          gprsNames[rs1].ccName, gprsNames[rs2].ccName, imm);
  return 1;
}

static int diss_SLLI_RV64I(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  shamt &= 0x3F;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSLLI \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, shamt);
  return 1;
}

static int diss_SRLI_RV64I(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  shamt &= 0x3F;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSRLI \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, shamt);
  return 1;
}

static int diss_SRAI_RV64I(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  shamt &= 0x3F;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSRAI \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, shamt);
  return 1;
}

static int diss_ADDIW(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);

  fprintf(stderr, PRI_ADDR " (0x%08x) \tADDIW \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, imm);
  return 1;
}

static int diss_SLLIW(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  if (shamt & 0x20) {
    // raise illegal instruction here see page 24 of riscv-spec-2.0
    assert(0);
  }
  shamt &= 0x1F;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSLLIW \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, shamt);
  return 1;
}

static int diss_SRLIW(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  if (shamt & 0x20) {
    // raise illegal instruction here see page 24 of riscv-spec-2.0
    assert(0);
  }
  shamt &= 0x1F;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSRLIW \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, shamt);
  return 1;
}

static int diss_SRAIW(Register addr, Instruction ir) {
  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  if (shamt & 0x20) {
    // raise illegal instruction here see page 24 of riscv-spec-2.0
    assert(0);
  }
  shamt &= 0x1F;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSRAIW \t%s,%s," PRI_REG "\n", addr,
          ir.raw, gprsNames[rd].ccName, gprsNames[rs1].ccName, shamt);
  return 1;
}

static int diss_ADDW(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tADDW \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SUBW(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSUBW \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SLLW(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSLLW \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SRLW(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSRLW \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_SRAW(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tSRAW \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

// 32 M Extension
static int diss_MUL(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tMUL \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_MULH(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tMULH \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_MULHSU(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tMULHSU \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_MULHU(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tMULHU \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_DIV(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tDIV \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_DIVU(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tDIVU \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_REM(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tREM \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

static int diss_REMU(Register addr, Instruction ir) {
  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;

  fprintf(stderr, PRI_ADDR " (0x%08x) \tREMU \t%s,%s,%s\n", addr, ir.raw,
          gprsNames[rd].ccName, gprsNames[rs1].ccName, gprsNames[rs2].ccName);
  return 1;
}

int disassemble_one(Register addr, Instruction ir) {
  int rc = -1;
  switch (ir.common.opcode) {
  case 0x3:  // opcode:0000011(3) 5 variants
    switch (ir.I.funct3) {
    case 0x0:
      rc = diss_LB(addr, ir);  // LB rd,rs1,imm
      break;
    case 0x1:
      rc = diss_LH(addr, ir);  // LH rd,rs1,imm
      break;
    case 0x2:
      rc = diss_LW(addr, ir);  // LW rd,rs1,imm
      break;
    case 0x3:
      rc = diss_LD(addr, ir);  // LD rd,rs1,imm
      break;
    case 0x4:
      rc = diss_LBU(addr, ir);  // LBU rd,rs1,imm
      break;
    case 0x5:
      rc = diss_LHU(addr, ir);  // LHU rd,rs1,imm
      break;
    case 0x6:
      rc = diss_LWU(addr, ir);  // LWU rd,rs1,imm
      break;
    }
    break;
  case 0xF:  // opcode:0001111(F) 2 variants
    switch (ir.I.funct3) {
    case 0x0:
      rc = diss_FENCE(addr, ir);  // FENCE
      break;
    case 0x1:
      rc = diss_FENCEI(addr, ir);  // FENCEI
      break;
    }
    break;
  case 0x13:  // opcode:0010011(13) 9 variants
    switch (ir.I.funct3) {
    case 0x0:
      rc = diss_ADDI(addr, ir);  // ADDI rd,rs1,imm
      break;
    case 0x1: {
      Shamt s;
      s.raw = ir.raw;
      if (s.shamt64.imm == 0x0) {
        rc = diss_SLLI_RV64I(addr, ir);  // SLLI rd,rs1,shamt
        break;
      }
    } break;
    case 0x2:
      rc = diss_SLTI(addr, ir);  // SLTI rd,rs1,imm
      break;
    case 0x3:
      rc = diss_SLTIU(addr, ir);  // SLTIU rd,rs1,imm
      break;
    case 0x4:
      rc = diss_XORI(addr, ir);  // XORI rd,rs1,imm
      break;
    case 0x5: {
      Shamt s;
      s.raw = ir.raw;
      if (s.shamt64.imm == 0x0) {  // imm11:0 0000000
        rc = diss_SRLI_RV64I(addr, ir);  // SRLI rd,rs1,shamt
        break;
      }
      if (s.shamt64.imm == 0x10) {
        rc = diss_SRAI_RV64I(addr, ir);  // SRAI rd,rs1,shamt
        break;
      }
    } break;
    case 0x6:
      rc = diss_ORI(addr, ir);  // ORI rd,rs1,imm
      break;
    case 0x7:
      rc = diss_ANDI(addr, ir);  // ANDI rd,rs1,imm
      break;
    }
    break;
  case 0x17:  // opcode:0010111(17) 1 variants
    rc = diss_AUIPC(addr, ir);  // AUIPC rd,imm
    break;
  case 0x1B:  // opcode:0011011(1B) 4 variants
    switch (ir.I.funct3) {
    case 0x0:
      rc = diss_ADDIW(addr, ir);  // ADDIW rd,rs1,imm
      break;
    case 0x1: {
      Shamt sa;
      sa.raw = ir.raw;
      if (sa.shamt32.imm == 0x0) {  // imm11:0 0000000
        rc = diss_SLLIW(addr, ir);  // SLLIW rd,rs1,shamt
        break;
      }
    } break;
    case 0x5: {
      Shamt sa;
      sa.raw = ir.raw;
      if (sa.shamt32.imm == 0x0) {  // imm11:0 0000000
        rc = diss_SRLIW(addr, ir);  // SRLIW rd,rs1,shamt
        break;
      }
      if (sa.shamt32.imm == 0x20) {  // imm11:0 0100000
        rc = diss_SRAIW(addr, ir);  // SRAIW rd,rs1,shamt
        break;
      }
    } break;
    }
    break;
  case 0x23:  // opcode:0100011(23) 3 variants
    switch (ir.S.funct3) {
    case 0x0:
      rc = diss_SB(addr, ir);  // SB rs1,rs2,imm
      break;
    case 0x1:
      rc = diss_SH(addr, ir);  // SH rs1,rs2,imm
      break;
    case 0x2:
      rc = diss_SW(addr, ir);  // SW rs1,rs2,imm
      break;
    case 0x3:
      rc = diss_SD(addr, ir);  // SD rs1,rs2,imm
      break;
    }
    break;
  case 0x33:  // opcode:0110011(33) 10 variants
    switch (ir.R.funct3) {
    case 0x0:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_ADD(addr, ir);  // ADD rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x1) {  // funct7: 0000001
        rc = diss_MUL(addr, ir);  // MUL rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x20) {  // funct7: 0100000
        rc = diss_SUB(addr, ir);  // SUB rd,rs1,rs2
        break;
      }
      break;
    case 0x1:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_SLL(addr, ir);  // SLL rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x1) {  // funct7: 0000001
        rc = diss_MULH(addr, ir);  // MULH rd,rs1,rs2
        break;
      }
      break;
    case 0x2:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_SLT(addr, ir);  // SLT rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x1) {  // funct7: 0000001
        rc = diss_MULHSU(addr, ir);  // MULHSU rd,rs1,rs2
        break;
      }
      break;
    case 0x3:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_SLTU(addr, ir);  // SLTU rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x1) {  // funct7: 0000001
        rc = diss_MULHU(addr, ir);  // MULHU rd,rs1,rs2
        break;
      }
      break;
    case 0x4:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_XOR(addr, ir);  // XOR rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x1) {  // funct7: 0000001
        rc = diss_DIV(addr, ir);  // DIV rd,rs1,rs2
        break;
      }
      break;
    case 0x5:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_SRL(addr, ir);  // SRL rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x1) {  // funct7: 0000001
        rc = diss_DIVU(addr, ir);  // DIVU rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x20) {  // funct7: 0100000
        rc = diss_SRA(addr, ir);  // SRA rd,rs1,rs2
        break;
      }
      break;
    case 0x6:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_OR(addr, ir);  // OR rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x1) {  // funct7: 0000001
        rc = diss_REM(addr, ir);  // REM rd,rs1,rs2
        break;
      }
      break;
    case 0x7:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_AND(addr, ir);  // AND rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x1) {  // funct7: 0000001
        rc = diss_REMU(addr, ir);  // REMU rd,rs1,rs2
        break;
      }
      break;
    }
    break;
  case 0x37:  // opcode:0110111(37) 1 variants
    rc = diss_LUI(addr, ir);  // LUI rd,imm
    break;
  case 0x3B:  // opcode:0111011(3B) 5 variants
    switch (ir.R.funct3) {
    case 0x0:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_ADDW(addr, ir);  // ADDW rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x20) {  // funct7: 0100000
        rc = diss_SUBW(addr, ir);  // SUBW rd,rs1,rs2
        break;
      }
      break;
    case 0x1:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_SLLW(addr, ir);  // SLLW rd,rs1,rs2
        break;
      }
      break;
    case 0x5:
      if (ir.R.funct7 == 0x0) {  // funct7: 0000000
        rc = diss_SRLW(addr, ir);  // SRLW rd,rs1,rs2
        break;
      }
      if (ir.R.funct7 == 0x20) {  // funct7: 0100000
        rc = diss_SRAW(addr, ir);  // SRAW rd,rs1,rs2
        break;
      }
      break;
    }
    break;
  case 0x63:  // opcode:1100011(63) 6 variants
    switch (ir.B.funct3) {
    case 0x0:
      rc = diss_BEQ(addr, ir);  // BEQ rs1,rs2,imm
      break;
    case 0x1:
      rc = diss_BNE(addr, ir);  // BNE rs1,rs2,imm
      break;
    case 0x4:
      rc = diss_BLT(addr, ir);  // BLT rs1,rs2,imm
      break;
    case 0x5:
      rc = diss_BGE(addr, ir);  // BGE rs1,rs2,imm
      break;
    case 0x6:
      rc = diss_BLTU(addr, ir);  // BLTU rs1,rs2,imm
      break;
    case 0x7:
      rc = diss_BGEU(addr, ir);  // BGEU rs1,rs2,imm
      break;
    }
    break;
  case 0x67:  // opcode:1100111(67) 1 variants
    if (ir.I.funct3 == 0) rc = diss_JALR(addr, ir);  // JALR rd,rs1,imm
    break;
  case 0x6F:  // opcode:1101111(6F) 1 variants
    rc = diss_JAL(addr, ir);  // JAL rd,imm
    break;
  case 0x73:  // opcode:1110011(73) 8 variants
    switch (ir.I.funct3) {
    case 0x0: {
      if (ir.I.imm11_0 == 0x0) {  // imm11:0 000000000000
        rc = diss_SCALL(addr, ir);  // SCALL
        break;
      }
      if (ir.I.imm11_0 == 0x1) {  // imm11:0 000000000001
        rc = diss_SBREAK(addr, ir);  // SBREAK
        break;
      }
    } break;
    case 0x2:
      if (ir.I.imm11_0 == 0xC00) {  // imm11:0 110000000000
        rc = diss_RDCYCLE(addr, ir);  // RDCYCLE rd
        break;
      }
      if (ir.I.imm11_0 == 0xC80) {  // imm11:0 110010000000
        rc = diss_RDCYCLEH(addr, ir);  // RDCYCLEH rd
        break;
      }
      if (ir.I.imm11_0 == 0xC01) {  // imm11:0 110000000001
        rc = diss_RDTIME(addr, ir);  // RDTIME rd
        break;
      }
      if (ir.I.imm11_0 == 0xC81) {  // imm11:0 110010000001
        rc = diss_RDTIMEH(addr, ir);  // RDTIMEH rd
        break;
      }
      if (ir.I.imm11_0 == 0xC02) {  // imm11:0 110000000010
        rc = diss_RDINSTRET(addr, ir);  // RDINSTRET rd
        break;
      }
      if (ir.I.imm11_0 == 0xC82) {  // imm11:0 110010000010
        rc = diss_RDINSTRETH(addr, ir);  // RDINSTRETH rd
        break;
      }
      break;
    }
  }

  if (rc < 0) {
    fprintf(stderr, "ERROR: Unsupported Instruction 0x%08x (opcode=0x%x)\n",
            ir.raw, ir.common.opcode);
    assert(0);
  }

  return rc;
}

int disassemble(Address addr, uint32_t buf[], int len) {
  int rc;
  int i;
  for (i = 0; i < len; i++) {
    rc = disassemble_one(addr + (i * (sizeof(Instruction))),
                         (Instruction) {.raw = buf[i]});
    if (rc < 0)
      fprintf(stderr, PRI_ADDR ": %08x, \tDECODE ERROR\n", addr, buf[i]);
  }
  return len;
}
