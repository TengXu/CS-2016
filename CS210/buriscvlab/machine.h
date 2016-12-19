#ifndef __MACHINE_H__
#define __MACHINE_H__
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

struct Core {
  struct RegisterFile regFile;
  // Internal Registers used to implement ISA
  Instruction ir;
};

static inline void irSet(struct Core* c, Register val) { c->ir.raw = val; }

static inline Register irGet(struct Core* c) { return c->ir.raw; }

struct Machine {
  struct BURiscv_Info info;
  struct mem_device** devmappings;
  struct State {
    struct Cpu {
      struct Core core;
    } cpu;
    Byte memory[];
  } state;
};

// Phyiscal address space layout
// 0000 0000                       : Ram start
// 0000 0100 (TRAP_VECTOR_BASE)    : riscv vector table format defined by riscv
//                                 : priviledged ISA
//    ...                          : valid ram determined by input image size
//     X     (m->info.memSize - 1) : end of ram
//     Y     PAGE_ALIGN(X)         : io mem start
//     Z     (Y+IO_MAPPING_LEN - 1): io mem end
//  INVALID  (Z+1)                 : rest of memory is invalid
// FFFF FFFF
//
// A bunch of sanity static assertions to make sure our machine definition is
// consistent
//

#endif
