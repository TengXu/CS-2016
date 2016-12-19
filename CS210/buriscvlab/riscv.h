#ifndef __RISCV_H__
#define __RISCV_H__
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

// 32 x register + pca
#define NUM_GPRS (32 + 1)

typedef enum Priv { U_PL = 0, S_PL = 1, H_PL = 2, M_PL = 3 } PrivlegeLevels;

#define VectorPrivilegeShift 6

typedef enum {
  USER = U_PL << VectorPrivilegeShift,
  SUPERVISOR = S_PL << VectorPrivilegeShift,
  HYPERVISOR = H_PL << VectorPrivilegeShift,
  MACHINE = M_PL << VectorPrivilegeShift,
  NMI = 0xFC,
  RESET = 0x100
} VectorOffset;

typedef enum {
  x0 = 0,
  x1 = 1,
  x2 = 2,
  x3 = 3,
  x4 = 4,
  x5 = 5,
  x6 = 6,
  x7 = 7,
  x8 = 8,
  x9 = 9,
  x10 = 10,
  x11 = 11,
  x12 = 12,
  x13 = 13,
  x14 = 14,
  x15 = 15,
  x16 = 16,
  x17 = 17,
  x18 = 18,
  x19 = 19,
  x20 = 20,
  x21 = 21,
  x22 = 22,
  x23 = 23,
  x24 = 24,
  x25 = 25,
  x26 = 26,
  x27 = 27,
  x28 = 28,
  x29 = 29,
  x30 = 30,
  x31 = 31,
  pc = 32
} GPR;

typedef enum {
  zero = x0,
  ra = x1,
  sp = x2,
  gp = x3,
  tp = x4,
  t0 = x5,
  t1 = x6,
  t2 = x7,
  s0 = x8,
  s1 = x9,
  a0 = x10,
  a1 = x11,
  a2 = x12,
  a3 = x13,
  a4 = x14,
  a5 = x15,
  a6 = x16,
  a7 = x17,
  s2 = x18,
  s3 = x19,
  s4 = x20,
  s5 = x21,
  s6 = x22,
  s7 = x23,
  s8 = x24,
  s9 = x25,
  s10 = x26,
  s11 = x27,
  t3 = x28,
  t4 = x29,
  t5 = x30,
  t6 = x31,
} CCREG;

struct GprNames {
  int num;
  char* gprName;
  char* ccName;
};
extern struct GprNames gprsNames[NUM_GPRS];

typedef union {
  Register raw;
  struct {
    Register Extensions : 26;
    Register Zero : XLEN - 28;
    Register Base : 2;
  } bits;
} MCpuid;

enum Base { RV32I = 0, RV32E = 1, RV64I = 2, RV64E = 3 };

typedef union {
  Register raw;
  struct {
    Register Source : 16;
    Register Implementation : XLEN - 16;
  } bits;
} MImpid;

enum Sources {
  UNIMPLEMENTED = 0x0000,
  ROCKET = 0x0001,
  OPENSOURCE_FIRST = 0x0002,
  OPENSOURCE_LAST = 0x7FFE,
  EXTENSION0 = 0x7FFF,
  ANONYMOUS = 0x8000,
  PROPRIETARY_FIRST = 0x8001,
  PROPRIETARY_LAST = 0xFFFE,
  EXTENSION1 = 0xFFFF
};

typedef union {
  Register raw;
  struct {
    Register IE : 1;
    Register PRV : 2;
    Register IE1 : 1;
    Register PRV1 : 2;
    Register IE2 : 1;
    Register PRV2 : 2;
    Register IE3 : 1;
    Register PRV3 : 2;
    Register FS : 2;
    Register XS : 2;
    Register MPRV : 1;
    Register VM : 5;
    Register PAD0 : XLEN - 23;
    Register SD : 1;
  } bits;
} MStatus;

typedef union {
  Register raw;
  struct {
    Register ExceptationCode : 4;
    const Register Zero4 : XLEN - 5;
    Register Interrupt : 1;
  } bits;
} MCause;

enum NonInterruptCauses {
  INST_MISS_ALGND = 0,
  INST_ACCESS_FLT = 1,
  INST_ILLEGAL = 2,
  BREAKPOINT = 3,
  LD_MISS_ALGND = 4,
  LD_ACCESS_FLT = 5,
  SWAMO_MISS_ALGND = 6,
  SWAMO_ACCESS_FLT = 7,
  ENV_CALL_U = 8,
  ENV_CALL_S = 9,
  ENV_CALL_H = 10,
  ENV_CALL_M = 11
};

// programmer visible registers
struct RegisterFile {
  struct {
    Register reg[NUM_GPRS];
  } gprs;
  struct {
    struct {
      const MCpuid mcpuid;
      const MImpid mimpid;
      const Register mhartid;
      MStatus mstatus;
      const Register mtvec;
      const Register mtdeleg;
      MIp mip;
      MIe mie;
      Register mtime;
      Register mtimecmp;
      Register mscratch;
      Register mepc;
      MCause mcause;
    } machine;
  } csrs;
};

static inline void setGpr(struct RegisterFile* rf, GPR gpr, Register val) {
  // writes to x[0] silently ignored
  if (gpr != x0) rf->gprs.reg[gpr] = val;
}

static inline Register getGpr(struct RegisterFile* rf, GPR gpr) {
  if (gpr == x0)
    return 0;
  else
    return rf->gprs.reg[gpr];
}

#define DEFINE_GPR_METHODS(REG)                                                \
  static inline void REG##Set(struct RegisterFile* rf, Register val) {         \
    setGpr(rf, REG, val);                                                      \
  }                                                                            \
                                                                               \
  static inline Register REG##Get(struct RegisterFile* rf) {                   \
    return getGpr(rf, REG);                                                    \
  }

static inline Register mtvecGet(struct RegisterFile* rf) {
  return rf->csrs.machine.mtvec;
}

DEFINE_GPR_METHODS(pc)

#endif
