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
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "common.h"

#include "tty.h"

static void console_storeByte(void* ptr, struct Machine* m, Address addr,
                              Byte v) __attribute__((unused));
static void console_loadByte(void* ptr, struct Machine* m, Address addr,
                             Byte* v) __attribute__((unused));
static void badcon_storeByte(void* ptr, struct Machine* m, Address addr, Byte v)
    __attribute__((unused));
static void badcon_loadByte(void* ptr, struct Machine* m, Address addr, Byte* v)
    __attribute__((unused));
static void badcon_storeHalfWord(void* ptr, struct Machine* m, Address addr,
                                 HalfWord v) __attribute__((unused));
static void badcon_loadHalfWord(void* ptr, struct Machine* m, Address addr,
                                HalfWord* v) __attribute__((unused));
static void badcon_storeWord(void* ptr, struct Machine* m, Address addr, Word v)
    __attribute__((unused));
static void badcon_loadWord(void* ptr, struct Machine* m, Address addr, Word* v)
    __attribute__((unused));
static void badcon_storeDoubleWord(void* ptr, struct Machine* m, Address addr,
                                   DoubleWord v) __attribute__((unused));
static void badcon_loadDoubleWord(void* ptr, struct Machine* m, Address addr,
                                  DoubleWord* v) __attribute__((unused));



struct console {
  int last_store_val;
  int num_store;
  int last_load_val;
  int num_load;
  int load_fd;
  int store_fd;
};

static inline Address consoleAddr(struct Machine* m) {
  return m->info.ioBaseAddr + BURISCV_CONSOLE_IO_ADDR_OFFSET;
}

static void console_storeByte(void* ptr, struct Machine* m, Address addr,
                              Byte v) {
  int rc;
  struct console* c;

  assert(ptr && addr == consoleAddr(m));
  c = ptr;
retry:
  rc = write(c->store_fd, &v, 1);
  if (rc == 1) {
    c->last_store_val = v;
    c->num_store++;
  } else {
    if (rc == -1 && errno == EINTR) goto retry;
  }
}

static void console_loadByte(void* ptr, struct Machine* m, Address addr,
                             Byte* v) {
  int rc;
  struct console* c;

  assert(ptr && addr == consoleAddr(m));
  c = ptr;
retry:
  rc = read(c->load_fd, &(c->last_load_val), 1);
  if (rc == 1) {
    c->num_load++;
    *v = c->last_load_val;
  } else {
    if (rc == -1 && errno == EINTR) goto retry;
    *v = 0;
  }
}

static void badcon_storeByte(void* ptr, struct Machine* m, Address addr,
                             Byte v) {
  assert(0);
}

static void badcon_loadByte(void* ptr, struct Machine* m, Address addr,
                            Byte* v) {
  assert(0);
}

static void badcon_storeHalfWord(void* ptr, struct Machine* m, Address addr,
                                 HalfWord v) {
  assert(0);
}

static void badcon_loadHalfWord(void* ptr, struct Machine* m, Address addr,
                                HalfWord* v) {
  assert(0);
}

static void badcon_storeWord(void* ptr, struct Machine* m, Address addr,
                             Word v) {
  assert(0);
}

static void badcon_loadWord(void* ptr, struct Machine* m, Address addr,
                            Word* v) {
  assert(0);
}

static void badcon_storeDoubleWord(void* ptr, struct Machine* m, Address addr,
                                   DoubleWord v) {
  assert(0);
}

static void badcon_loadDoubleWord(void* ptr, struct Machine* m, Address addr,
                                  DoubleWord* v) {
  assert(0);
}

int console_init(struct Machine* m) {
  struct console* c;
  Address addr;

  c = (struct console*)malloc(sizeof(struct console));
  if (c == NULL) return 0;

  bzero(c, sizeof(struct console));
  c->load_fd = STDIN_FILENO;
  c->store_fd = STDOUT_FILENO;

  tty_init(c->store_fd);
  int rc = mem_map_device(m, &addr, c, console_loadByte, console_storeByte,
                          badcon_loadHalfWord, badcon_storeHalfWord,
                          badcon_loadWord, badcon_storeWord,
                          badcon_loadDoubleWord, badcon_storeDoubleWord);

  if (rc >= 0) {
    assert(consoleAddr(m) == addr);
    VPRINT("mapping console at 0x%lx\n", addr);
  }
  return rc;
}
