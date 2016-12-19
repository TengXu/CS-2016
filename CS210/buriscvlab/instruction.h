#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__
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

typedef enum {
  INVALIDAM = 0,  // SYNTHETIC INVALID ADDRESSING MODE
  R,  // R-type
  I,  // I-type
  S,  // S-type
  B,  // SB-type
  U,  // U-type
  J,  // J-type
} InstType;

// risv-spec-v2.0 page 12 Figure 2.4
typedef union {
  uint32_t raw;
  struct {
    uint32_t field0 : 1;  // inst[20]
    uint32_t field1 : 4;  // inst[24:21]
    uint32_t field2 : 6;  // inst[30:25]
    uint32_t field3 : 21;  // inst[31]
  } I;
  struct {
    uint32_t field0 : 1;  // inst[7]
    uint32_t field1 : 4;  // inst[11:8]
    uint32_t field2 : 6;  // inst[30:25]
    uint32_t field3 : 21;  // inst[31]
  } S;
  struct {
    uint32_t field0 : 1;  // 0
    uint32_t field1 : 4;  // inst[11:8]
    uint32_t field2 : 6;  // inst[30:25]
    uint32_t field3 : 1;  // inst[7]
    uint32_t field4 : 20;  // inst[31]

  } B;
  struct {
    uint32_t field0 : 12;  // 0
    uint32_t field1 : 20;  // inst[31:12]
  } U;
  struct {
    uint32_t field0 : 1;  // 0
    uint32_t field1 : 10;  // inst[10:1]
    uint32_t field2 : 1;  // inst[11]
    uint32_t field3 : 8;  // inst[19:12]
    uint32_t field4 : 12;  // inst[20]
  } J;
} Immediate;

typedef union {
  uint32_t raw;
  struct {
    uint32_t opcode : 7;
    uint32_t pad : 32 - 7;
  } common;
  struct {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t funct7 : 7;
  } R;
  struct {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t imm11_0 : 12;
  } I;
  struct {
    uint32_t opcode : 7;
    uint32_t imm0 : 1;
    uint32_t imm4_1 : 4;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t imm10_5 : 6;
    uint32_t imm11 : 1;
  } S;
  struct {
    uint32_t opcode : 7;
    uint32_t imm11 : 1;
    uint32_t imm4_1 : 4;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t imm10_5 : 6;
    uint32_t imm12 : 1;
  } B;
  struct {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t imm31_12 : 20;
  } U;
  struct {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t imm19_12 : 8;
    uint32_t imm11 : 1;
    uint32_t imm10_1 : 10;
    uint32_t imm20 : 1;
  } J;
} Instruction;

typedef union {
  uint32_t raw;
  struct {
    uint32_t pad : 20;
    uint32_t shamt5 : 5;
    uint32_t imm : 7;
  } shamt32;
  struct {
    uint32_t pad : 20;
    uint32_t shamt6 : 6;
    uint32_t imm : 6;
  } shamt64;
} Shamt;

typedef enum {
  INST_EXIT = -4,
  INST_EXCEPTION = -3,
  INST_UNSUPPORTED = -2,
  INST_NYI = -1,
  INST_PC_UNSET = 1,
  INST_PC_SET = 2
} INST_RC;

#define signExtendToRegister(x, y)                                             \
  {                                                                            \
    if (sizeof(x) == 1)                                                        \
      y = (Register)((SRegister)((int8_t)x));                                  \
    else if (sizeof(x) == 2)                                                   \
      y = (Register)((SRegister)((int16_t)x));                                 \
    else if (sizeof(x) == 4)                                                   \
      y = (Register)((SRegister)((int32_t)x));                                 \
    else if (sizeof(x) == 8)                                                   \
      y = (Register)((SRegister)((int64_t)x));                                 \
    else                                                                       \
      assert(0);                                                               \
  }

#define zeroExtendToRegister(x, y)                                             \
  {                                                                            \
    if (sizeof(x) == 1)                                                        \
      y = (Register)((uint8_t)x);                                              \
    else if (sizeof(x) == 2)                                                   \
      y = (Register)((uint16_t)x);                                             \
    else if (sizeof(x) == 4)                                                   \
      y = (Register)((uint32_t)x);                                             \
    else if (sizeof(x) == 8)                                                   \
      y = (Register)((uint64_t)x);                                             \
    else                                                                       \
      assert(0);                                                               \
  }

struct Machine;
Register decodeImmediate(Instruction ir, InstType t);
int inst_loop(struct Machine* m, int count);

#endif
