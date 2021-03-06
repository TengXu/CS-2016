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

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

struct GprNames gprsNames[NUM_GPRS] = {{0, "x0", "zero"},
                                       {1, "x1", "ra"},
                                       {2, "x2", "sp"},
                                       {3, "x3", "gp"},
                                       {4, "x4", "tp"},
                                       {5, "x5", "t0"},
                                       {6, "x6", "t1"},
                                       {7, "x7", "t2"},
                                       {8, "x8", "s0"},
                                       {9, "x9", "s1"},
                                       {10, "x10", "a0"},
                                       {11, "x11", "a1"},
                                       {12, "x12", "a2"},
                                       {13, "x13", "a3"},
                                       {14, "x14", "a4"},
                                       {15, "x15", "a5"},
                                       {16, "x16", "a6"},
                                       {17, "x17", "a7"},
                                       {18, "x18", "s2"},
                                       {19, "x19", "s3"},
                                       {20, "x20", "s4"},
                                       {21, "x21", "s5"},
                                       {22, "x22", "s6"},
                                       {23, "x23", "s7"},
                                       {24, "x24", "s8"},
                                       {25, "x25", "s9"},
                                       {26, "x26", "s10"},
                                       {27, "x27", "s11"},
                                       {28, "x28", "t3"},
                                       {29, "x29", "t4"},
                                       {30, "x30", "t5"},
                                       {31, "x31", "t6"},
                                       {32, "pc", "pc"}};

Register decodeImmediate(Instruction ir, InstType t) {
  Register val;
  Immediate imm;
  imm.raw = 0;

  switch (t) {
  case R: { break; }
  case I: {
    int32_t signed_imm = ir.raw;
    signed_imm = signed_imm >> 20;
    imm.raw = signed_imm;
  } break;
  case S:
    imm.S.field0 = ir.S.imm0;
    imm.S.field1 = ir.S.imm4_1;
    imm.S.field2 = ir.S.imm10_5;
    if (ir.S.imm11) imm.S.field3 = ~(imm.S.field3);
    break;
  case B:
    imm.B.field0 = 0;
    imm.B.field1 = ir.B.imm4_1;
    imm.B.field2 = ir.B.imm10_5;
    imm.B.field3 = ir.B.imm11;
    if (ir.B.imm12) imm.B.field4 = ~(imm.B.field4);
    break;
  case U:
    imm.U.field0 = 0;
    imm.U.field1 = ir.U.imm31_12;
    break;
  case J:
    imm.J.field0 = 0;
    imm.J.field1 = ir.J.imm10_1;
    imm.J.field2 = ir.J.imm11;
    imm.J.field3 = ir.J.imm19_12;
    if (ir.J.imm20) imm.J.field4 = ~(imm.J.field4);
    break;
  default:
    assert(0);
  }
  signExtendToRegister(imm.raw, val);
  return val;
}

static INST_RC inst_LUI(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.U.rd;
  Register imm = decodeImmediate(ir, U);
    
  setGpr(rf, rd, imm);
  return INST_PC_UNSET;

}


static INST_RC inst_AUIPC(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.U.rd;
  Register imm = decodeImmediate(ir, U);
  
  Register newpc = pcGet(rf) + imm;
  setGpr(rf,rd,newpc);
  return INST_PC_UNSET;

}

static INST_RC inst_JAL(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.J.rd;
  Register imm = decodeImmediate(ir, J);
  
  Register oldpc = pcGet(rf);
  Register newpc = oldpc + imm;
  Register link = oldpc + (Register)sizeof(Instruction);

  setGpr(rf,rd,link);
  pcSet(rf,newpc);
  return INST_PC_SET;

}

static INST_RC inst_JALR(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  
  Register oldpc = pcGet(rf);
  Register value = getGpr(rf, rs1);
  Register newpc = imm + value;
  Register link = oldpc + (Register)sizeof(Instruction);
  
  setGpr(rf, rd, link);
  pcSet(rf, newpc);
  return INST_PC_SET;
}

static INST_RC inst_BEQ(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);
  
  Register rs1Val = getGpr(rf, rs1);
  Register rs2Val = getGpr(rf, rs2);
  SRegister newpc = pcGet(rf) + imm;
  
  if (rs1Val == rs2Val){
    pcSet(rf,newpc);
    return INST_PC_SET;
  }
  return INST_PC_UNSET;
}


// self edit
static INST_RC inst_BNE(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);
  
  SRegister rs1Val = getGpr(rf, rs1);
  SRegister rs2Val = getGpr(rf, rs2);
  Register newpc = pcGet(rf) + imm;

  if (rs1Val != rs2Val ) {
    pcSet(rf, newpc);
    return INST_PC_SET;
  }
  return INST_PC_UNSET;
}

/*
static INST_RC inst_BNE(struct Machine* m) {
    struct Core* core = &(m->state.cpu.core);
    struct RegisterFile* rf = &(core->regFile);
    Instruction ir;
    ir.raw = irGet(core);
    
    uint8_t rs1 = ir.B.rs1;
    uint8_t rs2 = ir.B.rs2;
    Register imm = decodeImmediate(ir, B);
    NYI;
    assert(0);
    return INST_NYI;
    
}
*/

static INST_RC inst_BLT(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);
  
  SRegister rs1Val = getGpr(rf,rs1);
  SRegister rs2Val = getGpr(rf,rs2);
  Register newpc = pcGet(rf) + imm;

  if (rs1Val < rs2Val){
    pcSet(rf,newpc);
    return INST_PC_SET;
  }
  return INST_PC_UNSET;
}

static INST_RC inst_BGE(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);
  
  SRegister rs1Val = getGpr(rf, rs1);
  SRegister rs2Val = getGpr(rf, rs2);
  Register newpc = pcGet(rf) + imm;

  if (rs1Val >= rs2Val){
    pcSet(rf, newpc);
    return INST_PC_SET;
  }
  return INST_PC_UNSET;
}

static INST_RC inst_BLTU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);
  
  SRegister rs1Val = getGpr(rf,rs1);
  SRegister rs2Val = getGpr(rf,rs2);
  Register newpc = pcGet(rf) + imm;

  if (rs1Val < rs2Val){
    pcSet(rf,newpc);
    return INST_PC_SET;
  }
  return INST_PC_UNSET;

}

static INST_RC inst_BGEU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.B.rs1;
  uint8_t rs2 = ir.B.rs2;
  Register imm = decodeImmediate(ir, B);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_LB(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  
  Register rs1Val = getGpr(rf,rs1);
  Address eaddr = rs1Val + imm;

  Byte b = mem_getByte(m,eaddr);
  Register value;
  signExtendToRegister(b,value);
  setGpr(rf, rd, value);
  return INST_PC_UNSET;
}

static INST_RC inst_LH(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_LW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  
  Register rs1Val = getGpr(rf, rs1);
  Address eaddr = rs1Val + imm;

  Word b = mem_getWord(m, eaddr);
  Register value;
  signExtendToRegister(b, value);
  setGpr(rf, rd, value);
  return INST_PC_UNSET;
}


// self edit
static INST_RC inst_LBU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  
  Register rs1Val = getGpr(rf, rs1);
  Address eaddr = rs1Val + imm;
  Byte b = mem_getByte(m, eaddr);
  Register value;
  signExtendToRegister(b, value);
  setGpr(rf, rd, value);
  return INST_PC_UNSET;
}


/*
static INST_RC inst_LBU(struct Machine* m) {
    struct Core* core = &(m->state.cpu.core);
    struct RegisterFile* rf = &(core->regFile);
    Instruction ir;
    ir.raw = irGet(core);
    
    uint8_t rd = ir.I.rd;
    uint8_t rs1 = ir.I.rs1;
    Register imm = decodeImmediate(ir, I);
    NYI;
    assert(0);
    return INST_NYI;
    
}
*/

static INST_RC inst_LHU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SB(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.S.rs1;
  uint8_t rs2 = ir.S.rs2;
  Register imm = decodeImmediate(ir, S);
  
  Register rs1Val = getGpr(rf,rs1);
  Address eaddr = rs1Val + imm;

  uint8_t value = getGpr(rf,rs2);
  mem_putByte(m,eaddr,value);
  return INST_PC_UNSET;

}

static INST_RC inst_SH(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.S.rs1;
  uint8_t rs2 = ir.S.rs2;
  Register imm = decodeImmediate(ir, S);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.S.rs1;
  uint8_t rs2 = ir.S.rs2;
  Register imm = decodeImmediate(ir, S);
  
  Register rs1Val = getGpr(rf, rs1);
  Address eaddr = rs1Val + imm;
  Word value = getGpr(rf, rs2);
  mem_putWord(m, eaddr, value);
  
  return INST_PC_UNSET;
}


/*
static INST_RC inst_SW(struct Machine* m) {
    struct Core* core = &(m->state.cpu.core);
    struct RegisterFile* rf = &(core->regFile);
    Instruction ir;
    ir.raw = irGet(core);
    
    uint8_t rs1 = ir.S.rs1;
    uint8_t rs2 = ir.S.rs2;
    Register imm = decodeImmediate(ir, S);
    NYI;
    assert(0);
    return INST_NYI;
    
}
*/
 

//self edit
static INST_RC inst_ADDI(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  
  
  Register rs1Val = getGpr(rf, rs1);
  //uint32_t v32 = (uint32_t)(rs1Val + imm);
  SRegister value = (rs1Val + imm);

  setGpr(rf, rd, value);
  return INST_PC_UNSET;
}



static INST_RC inst_SLTI(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SLTIU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_XORI(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_ORI(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_ANDI(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_ADD(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  
  Register rs1Val = getGpr(rf, rs1);
  Register rs2Val = getGpr(rf, rs2);
  SRegister value = (rs1Val + rs2Val);
  
  setGpr(rf, rd, value);
  return INST_PC_UNSET;
}

static INST_RC inst_SUB(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SLL(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SLT(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SLTU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_XOR(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SRL(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SRA(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_OR(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_AND(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  
  Register rs1Val = getGpr(rf, rs1);
  Register rs2Val = getGpr(rf, rs2);
  SRegister value = (rs1Val & rs2Val);

  setGpr(rf, rd, value);
  return INST_PC_UNSET;

}

static INST_RC inst_FENCE(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_FENCEI(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SCALL(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);






  NYI;
  assert(0);
  return INST_NYI;

}

//self edit
static INST_RC inst_SBREAK(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);
  return INST_EXIT;
}

static INST_RC inst_RDCYCLE(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_RDCYCLEH(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_RDTIME(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_RDTIMEH(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_RDINSTRET(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_RDINSTRETH(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;






  NYI;
  assert(0);
  return INST_NYI;

}


static INST_RC inst_LWU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_LD(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  
  Register rs1Val = getGpr(rf, rs1);
  Address eaddr = rs1Val + imm;

  DoubleWord b = mem_getWord(m, eaddr);
  Register value;
  signExtendToRegister(b, value);
  setGpr(rf, rd, value);
  return INST_PC_UNSET; 
}

static INST_RC inst_SD(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rs1 = ir.S.rs1;
  uint8_t rs2 = ir.S.rs2;
  Register imm = decodeImmediate(ir, S);
  
  Register rs1Val = getGpr(rf, rs1);
  Address eaddr = rs1Val + imm;

  DoubleWord value = getGpr(rf, rs2);
  mem_putWord(m, eaddr, value);
  return INST_PC_UNSET;
}

static INST_RC inst_SLLI_RV64I(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  shamt &= 0x3F;
  
  Register rs1Val = getGpr(rf, rs1);
  Register value = rs1Val << shamt; 
  
  setGpr(rf, rd, value);
  return INST_PC_UNSET;
}

static INST_RC inst_SRLI_RV64I(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  shamt &= 0x3F;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SRAI_RV64I(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  shamt &= 0x3F;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_ADDIW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register imm = decodeImmediate(ir, I);
  
  Register rs1Val = getGpr(rf, rs1);
  uint32_t v32 = (uint32_t)(rs1Val + imm);
  Register value;
  signExtendToRegister(v32, value);
  
  setGpr(rf, rd, value);
  return INST_PC_UNSET;

}

static INST_RC inst_SLLIW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  if (shamt & 0x20) {

    assert(0);
  }
  shamt &= 0x1F;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SRLIW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  if (shamt & 0x20) {

    assert(0);
  }
  shamt &= 0x1F;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SRAIW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.I.rd;
  uint8_t rs1 = ir.I.rs1;
  Register shamt = decodeImmediate(ir, I);
  if (shamt & 0x20) {

    assert(0);
  }
  shamt &= 0x1F;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_ADDW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SUBW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  
  Register rs1Val = getGpr(rf, rs1);
  Register rs2Val = getGpr(rf, rs2);
  Word value = rs1Val - rs2Val;

  setGpr(rf, rd, value);
  return INST_PC_UNSET;
}

static INST_RC inst_SLLW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SRLW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_SRAW(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}


static INST_RC inst_MUL(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_MULH(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_MULHSU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_MULHU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_DIV(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_DIVU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_REM(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;






  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_REMU(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(core->regFile);
  Instruction ir;
  ir.raw = irGet(core);

  uint8_t rd = ir.R.rd;
  uint8_t rs1 = ir.R.rs1;
  uint8_t rs2 = ir.R.rs2;
  NYI;
  assert(0);
  return INST_NYI;

}

static INST_RC inst_decodeAndExecute(struct Machine* m) {
  int rc = INST_UNSUPPORTED;
  Instruction ir = m->state.cpu.core.ir;

  switch (ir.common.opcode) {
  case 0x3:
    switch (ir.I.funct3) {
    case 0x0:
      rc = inst_LB(m);
      break;
    case 0x1:
      rc = inst_LH(m);
      break;
    case 0x2:
      rc = inst_LW(m);
      break;
    case 0x3:
      rc = inst_LD(m);
      break;
    case 0x4:
      rc = inst_LBU(m);
      break;
    case 0x5:
      rc = inst_LHU(m);
      break;
    case 0x6:
      rc = inst_LWU(m);
      break;
    }
    break;
  case 0xF:
    switch (ir.I.funct3) {
    case 0x0:
      rc = inst_FENCE(m);
      break;
    case 0x1:
      rc = inst_FENCEI(m);
      break;
    }
    break;
  case 0x13:
    switch (ir.I.funct3) {
    case 0x0:
      rc = inst_ADDI(m);
      break;
    case 0x1: {
      Shamt s;
      s.raw = ir.raw;
      if (s.shamt64.imm == 0x0) {
        rc = inst_SLLI_RV64I(m);
        break;
      }
    } break;
    case 0x2:
      rc = inst_SLTI(m);
      break;
    case 0x3:
      rc = inst_SLTIU(m);
      break;
    case 0x4:
      rc = inst_XORI(m);
      break;
    case 0x5: {
      Shamt s;
      s.raw = ir.raw;
      if (s.shamt64.imm == 0x0) {
        rc = inst_SRLI_RV64I(m);
        break;
      }
      if (s.shamt64.imm == 0x10) {
        rc = inst_SRAI_RV64I(m);
        break;
      }
    } break;
    case 0x6:
      rc = inst_ORI(m);
      break;
    case 0x7:
      rc = inst_ANDI(m);
      break;
    }
    break;
  case 0x17:
    rc = inst_AUIPC(m);
    break;
  case 0x1B:
    switch (ir.I.funct3) {
    case 0x0:
      rc = inst_ADDIW(m);
      break;
    case 0x1: {
      Shamt sa;
      sa.raw = ir.raw;
      if (sa.shamt32.imm == 0x0) {
        rc = inst_SLLIW(m);
        break;
      }
    } break;
    case 0x5: {
      Shamt sa;
      sa.raw = ir.raw;
      if (sa.shamt32.imm == 0x0) {
        rc = inst_SRLIW(m);
        break;
      }
      if (sa.shamt32.imm == 0x20) {
        rc = inst_SRAIW(m);
        break;
      }
    } break;
    }
    break;
  case 0x23:
    switch (ir.S.funct3) {
    case 0x0:
      rc = inst_SB(m);
      break;
    case 0x1:
      rc = inst_SH(m);
      break;
    case 0x2:
      rc = inst_SW(m);
      break;
    case 0x3:
      rc = inst_SD(m);
      break;
    }
    break;
  case 0x33:
    switch (ir.R.funct3) {
    case 0x0:
      if (ir.R.funct7 == 0x0) {
        rc = inst_ADD(m);
        break;
      }
      if (ir.R.funct7 == 0x1) {
        rc = inst_MUL(m);
        break;
      }
      if (ir.R.funct7 == 0x20) {
        rc = inst_SUB(m);
        break;
      }
      break;
    case 0x1:
      if (ir.R.funct7 == 0x0) {
        rc = inst_SLL(m);
        break;
      }
      if (ir.R.funct7 == 0x1) {
        rc = inst_MULH(m);
        break;
      }
      break;
    case 0x2:
      if (ir.R.funct7 == 0x0) {
        rc = inst_SLT(m);
        break;
      }
      if (ir.R.funct7 == 0x1) {
        rc = inst_MULHSU(m);
        break;
      }
      break;
    case 0x3:
      if (ir.R.funct7 == 0x0) {
        rc = inst_SLTU(m);
        break;
      }
      if (ir.R.funct7 == 0x1) {
        rc = inst_MULHU(m);
        break;
      }
      break;
    case 0x4:
      if (ir.R.funct7 == 0x0) {
        rc = inst_XOR(m);
        break;
      }
      if (ir.R.funct7 == 0x1) {
        rc = inst_DIV(m);
        break;
      }
      break;
    case 0x5:
      if (ir.R.funct7 == 0x0) {
        rc = inst_SRL(m);
        break;
      }
      if (ir.R.funct7 == 0x1) {
        rc = inst_DIVU(m);
        break;
      }
      if (ir.R.funct7 == 0x20) {
        rc = inst_SRA(m);
        break;
      }
      break;
    case 0x6:
      if (ir.R.funct7 == 0x0) {
        rc = inst_OR(m);
        break;
      }
      if (ir.R.funct7 == 0x1) {
        rc = inst_REM(m);
        break;
      }
      break;
    case 0x7:
      if (ir.R.funct7 == 0x0) {
        rc = inst_AND(m);
        break;
      }
      if (ir.R.funct7 == 0x1) {
        rc = inst_REMU(m);
        break;
      }
      break;
    }
    break;
  case 0x37:
    rc = inst_LUI(m);
    break;
  case 0x3B:
    switch (ir.R.funct3) {
    case 0x0:
      if (ir.R.funct7 == 0x0) {
        rc = inst_ADDW(m);
        break;
      }
      if (ir.R.funct7 == 0x20) {
        rc = inst_SUBW(m);
        break;
      }
      break;
    case 0x1:
      if (ir.R.funct7 == 0x0) {
        rc = inst_SLLW(m);
        break;
      }
      break;
    case 0x5:
      if (ir.R.funct7 == 0x0) {
        rc = inst_SRLW(m);
        break;
      }
      if (ir.R.funct7 == 0x20) {
        rc = inst_SRAW(m);
        break;
      }
      break;
    }
    break;
  case 0x63:
    switch (ir.B.funct3) {
    case 0x0:
      rc = inst_BEQ(m);
      break;
    case 0x1:
      rc = inst_BNE(m);
      break;
    case 0x4:
      rc = inst_BLT(m);
      break;
    case 0x5:
      rc = inst_BGE(m);
      break;
    case 0x6:
      rc = inst_BLTU(m);
      break;
    case 0x7:
      rc = inst_BGEU(m);
      break;
    }
    break;
  case 0x67:
    if (ir.I.funct3 == 0) rc = inst_JALR(m);
    break;
  case 0x6F:
    rc = inst_JAL(m);
    break;
  case 0x73:
    switch (ir.I.funct3) {
    case 0x0: {
      if (ir.I.imm11_0 == 0x0) {
        rc = inst_SCALL(m);
        break;
      }
      if (ir.I.imm11_0 == 0x1) {
        rc = inst_SBREAK(m);
        break;
      }
    } break;
    case 0x2:
      if (ir.I.imm11_0 == 0xC00) {
        rc = inst_RDCYCLE(m);
        break;
      }
      if (ir.I.imm11_0 == 0xC80) {
        rc = inst_RDCYCLEH(m);
        break;
      }
      if (ir.I.imm11_0 == 0xC01) {
        rc = inst_RDTIME(m);
        break;
      }
      if (ir.I.imm11_0 == 0xC81) {
        rc = inst_RDTIMEH(m);
        break;
      }
      if (ir.I.imm11_0 == 0xC02) {
        rc = inst_RDINSTRET(m);
        break;
      }
      if (ir.I.imm11_0 == 0xC82) {
        rc = inst_RDINSTRETH(m);
        break;
      }
      break;
    }
  }

  if (rc == INST_PC_UNSET) {
    struct RegisterFile* rf = &(m->state.cpu.core.regFile);
    pcSet(rf, pcGet(rf) + sizeof(Instruction));
    return rc;
  }

  if (rc == INST_UNSUPPORTED) {
    fprintf(stderr, "ERROR: Unsupported Instruction 0x%08x (opcode=0x%x)\n",
            ir.raw, ir.common.opcode);
    assert(0);
  }

  return rc;
}

static inline int inst_fetch(struct Machine* m) {
  struct Core* core = &(m->state.cpu.core);
  struct RegisterFile* rf = &(m->state.cpu.core.regFile);
  irSet(core, mem_getWord(m, pcGet(rf)));
  return 1;
}

int inst_loop(struct Machine* m, int count) {
  int rc = 1;

  if (count < 0) return rc;

  while (1) {
    TRACE_LOOP(dump_nextInstructionState(m));

    rc = inst_fetch(m);
    if (rc >= 0) rc= inst_decodeAndExecute(m);

    m->info.instCount++;
    if (rc < 0 || (count && m->info.instCount == count)) break;
  }

  VPRINT("EXITING: count=%llu\n", m->info.instCount);
  return rc;
}
