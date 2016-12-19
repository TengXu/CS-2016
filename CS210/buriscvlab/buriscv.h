#ifndef __BURISCV_H__
#define __BURISCV_H__
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

#define MIN_PHY_ADDRESS_BITS 16
#define MIN_PHY_MEMSIZE (1 << MIN_PHY_ADDRESS_BITS)
#define MAX_PHY_ADDRESS_BITS 20
#define MAX_PHY_MEMSIZE (1 << MAX_PHY_ADDRESS_BITS)
#define TRAP_VECTOR_BASE 0x00000100
#define IO_BASE_ALIGNMENT BURISCV_PAGE_LEN
#define IO_MAPPING_MAX_LEN BURISCV_PAGE_LEN

// Riscv Registers with customized fields
typedef union {
  Register raw;
  struct {
    const Register Zero0 : 1;
    Register SSIP : 1;
    Register HSIP : 1;
    Register MSIP : 1;
    const Register Zero4 : 1;
    const Register STIP : 1;
    const Register HTIP : 1;
    const Register MTIP : 1;
    const Register Zero8 : 1;
    const Register SHIP : 1;  // BURISC Supervisor External IRQ Pin
    const Register HHIP : 1;  // BURISC Hypervisor External IRQ Pin
    const Register MHIP : 1;  // BURISC Machine    External IRQ Pin
    const Register Zero12 : XLEN - 12;
  } bits;
} MIp;

typedef union {
  Register raw;
  struct {
    const Register Zero0 : 1;
    Register SSIP : 1;
    Register HSIP : 1;
    Register MSIP : 1;
    const Register Zero4 : 1;
    const Register STIP : 1;
    const Register HTIP : 1;
    const Register MTIP : 1;
    const Register Zero8 : 1;
    const Register SEIP : 1;  // BURISC Supervisor External IRQ Pin
    const Register HEIP : 1;  // BURISC Hypervisor External IRQ Pin
    const Register MEIP : 1;  // BURISC Machine    External IRQ Pin
    const Register Zero12 : XLEN - 12;
  } bits;
} MIe;

enum InterruptCauses { SW_INT = 0, TIMER_INT = 1, BURISCV_HW_INT = 15 };

struct Machine;

extern int buriscv_machineInit(struct Machine** m, int imgfd);
#endif
