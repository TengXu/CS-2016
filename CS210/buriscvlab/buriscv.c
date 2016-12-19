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
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include "common.h"

struct Machine BURISCV_MACHINE_DEFAULTS = {
    .info = {//      .memSize    = sbuf.st_size,
             //      .ioBaseAddr = PAGE_ALIGN(sbuf.st_size),
             .ioLen = 0x0,  // increased when io mappings are created
             .instCount = 0, },
    .state = {
        .cpu = {
            .core = {
                .regFile = {
                    .gprs = {
#if 0
	    .reg = {
	      0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
	      0x8, 0x9, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
	      0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23,
	      0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31,
	      0xdeadbeef
	    },
#else
                        .reg = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                0x0, 0x0, 0x0, 0x0, 0x0, 0xdeadbeef},
#endif
                    },
                    .csrs = {
                        .machine = {
                            .mcpuid.bits =
                                {.Extensions = 0, .Zero = 0, .Base = RV32I},
                            .mimpid.bits = {
                                .Source = ANONYMOUS,
                                .Implementation =
                                    BURISCV_IMPLEMENTATION_VERSION},
                            /* "The mhartid register is an XLEN-bit read-only
                               register containing the integer ID of the
                               hardware
                               thread running the code. This register must be
                               readable in any implementation. Hart IDs might
                               not necessarily be numbered contiguously in a
                               multiprocessor system, but at least one hart must
                               have a hart ID of zero." p17 : 3.1.3 */
                            .mhartid = 0x0,
                            .mstatus.bits = {.IE = 0, .PRV = M_PL},
                            .mtvec = TRAP_VECTOR_BASE,
                            /* "The machine trap delegation register (mtdeleg)
                               is an XLEN-bit read/write register that must be
                               implemented, but which can contain a read-only
                               value of zero, indicating that hardware will
                               always direct all traps to machine mode. p25 :
                               3.1.10" */
                            .mtdeleg = 0x0, }, }, },
                .ir = {.raw = 0x0}, }, }, }, };

int buriscv_machineInit(struct Machine** m, int imgfd) {
  struct stat sbuf;

  if (fstat(imgfd, &sbuf) != 0) {
    perror(__func__);
    return 0;
  }

  if (sbuf.st_size < MIN_PHY_MEMSIZE) {
    fprintf(stderr, "memory image larger than max physical memory size\n");
    return -1;
  }

  if (sbuf.st_size > MAX_PHY_MEMSIZE) {
    fprintf(stderr, "memory image larger than max physical memory size\n");
    return -1;
  }

  *m = malloc(sizeof(struct Machine) + sbuf.st_size);
  if (*m == NULL) {
    fprintf(stderr, "failed to malloc machine structure\n");
    return -1;
  }

  memcpy(*m, &BURISCV_MACHINE_DEFAULTS, sizeof(struct Machine));

  (*m)->info.memSize = sbuf.st_size;
  (*m)->info.ioBaseAddr = BURISCV_PAGE_ALIGN(sbuf.st_size);

  return 1;
}
