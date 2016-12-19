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

// at the expense of memory we avoid
// searching data structures on load store
// path by maintaining a complete array of possible
// mappings
struct mem_device {
  void* devptr;
  memdev_loadByte lbfunc;
  memdev_storeByte sbfunc;
  memdev_loadHalfWord lhwfunc;
  memdev_storeHalfWord shwfunc;
  memdev_loadWord lwfunc;
  memdev_storeWord swfunc;
  memdev_loadDoubleWord ldwfunc;
  memdev_storeDoubleWord sdwfunc;
};

int mem_mappings_init(struct Machine* m) {
  m->devmappings = (struct mem_device**)malloc(sizeof(struct mem_device*) *
                                               IO_MAPPING_MAX_LEN);
  assert(m->devmappings);
  bzero(m->devmappings, sizeof(struct mem_device*) * IO_MAPPING_MAX_LEN);
  m->info.ioLen = 0;
  return 1;
}

int mem_map_device(struct Machine* m, Address* addr, void* devptr,
                   memdev_loadByte lbfunc, memdev_storeByte sbfunc,
                   memdev_loadHalfWord lhwfunc, memdev_storeHalfWord shwfunc,
                   memdev_loadWord lwfunc, memdev_storeWord swfunc,
                   memdev_loadDoubleWord ldwfunc,
                   memdev_storeDoubleWord sdwfunc) {
  struct mem_device* d;
  int i;

  if (m->info.ioLen >= IO_MAPPING_MAX_LEN) return -1;

  i = m->info.ioLen;
  m->info.ioLen++;
  *addr = m->info.ioBaseAddr + i;

  d = m->devmappings[i];
  if (d != NULL) return -1;

  d = (struct mem_device*)malloc(sizeof(struct mem_device));
  assert(d);

  d->devptr = devptr;
  d->lbfunc = lbfunc;
  d->sbfunc = sbfunc;
  d->lhwfunc = lhwfunc;
  d->shwfunc = shwfunc;
  d->lwfunc = lwfunc;
  d->swfunc = swfunc;
  d->ldwfunc = ldwfunc;
  d->sdwfunc = sdwfunc;

  m->devmappings[i] = d;

  return 1;
}

int mem_umap_device(struct Machine* m, Address addr, void** devptr) {
  NYI;
  return 1;
}

static struct mem_device* ioAddrToDev(struct Machine* m, Address a) {
  if (a >= m->info.ioBaseAddr && a < (m->info.ioBaseAddr + m->info.ioLen)) {
    int i = a - m->info.ioBaseAddr;
    return m->devmappings[i];
  }
  return NULL;
}

int mem_map_loadByte(struct Machine* m, Address addr, Byte* v) {
  struct mem_device* d = ioAddrToDev(m, addr);
  if (d) {
    if (d->lbfunc) d->lbfunc(d->devptr, m, addr, v);
    return 1;
  }
  return -1;
}

int mem_map_storeByte(struct Machine* m, Address addr, Byte v) {
  struct mem_device* d = ioAddrToDev(m, addr);
  if (d) {
    if (d->sbfunc) d->sbfunc(d->devptr, m, addr, v);
    return 1;
  }
  return -1;
}

int mem_map_loadHalfWord(struct Machine* m, Address addr, HalfWord* v) {
  // we allow Word operations to actually go past io range
  // eg. load of word on last byte of io range will still
  //     invoke load word of that device mapping
  // aka not doing validIOAddr(m,a+sizeof(Word))
  struct mem_device* d = ioAddrToDev(m, addr);
  if (d) {
    if (d->lhwfunc) d->lhwfunc(d->devptr, m, addr, v);
    return 1;
  }
  return -1;
}

int mem_map_storeHalfWord(struct Machine* m, Address addr, HalfWord v) {
  // we allow Word operations to actually go past io range
  // eg. load of word on last byte of io range will still
  //     invoke load word of that device mapping
  // aka not doing validIOAddr(m,a+sizeof(Word))
  struct mem_device* d = ioAddrToDev(m, addr);
  if (d) {
    if (d->shwfunc) d->shwfunc(d->devptr, m, addr, v);
    return 1;
  }
  return -1;
}

int mem_map_loadWord(struct Machine* m, Address addr, Word* v) {
  // we allow Word operations to actually go past io range
  // eg. load of word on last byte of io range will still
  //     invoke load word of that device mapping
  // aka not doing validIOAddr(m,a+sizeof(Word))
  struct mem_device* d = ioAddrToDev(m, addr);
  if (d) {
    if (d->lwfunc) d->lwfunc(d->devptr, m, addr, v);
    return 1;
  }
  return -1;
}

int mem_map_storeWord(struct Machine* m, Address addr, Word v) {
  // we allow Word operations to actually go past io range
  // eg. load of word on last byte of io range will still
  //     invoke load word of that device mapping
  // aka not doing validIOAddr(m,a+sizeof(Word))
  struct mem_device* d = ioAddrToDev(m, addr);
  if (d) {
    if (d->swfunc) d->swfunc(d->devptr, m, addr, v);
    return 1;
  }
  return -1;
}

int mem_map_loadDoubleWord(struct Machine* m, Address addr, DoubleWord* v) {
  // we allow Word operations to actually go past io range
  // eg. load of word on last byte of io range will still
  //     invoke load word of that device mapping
  // aka not doing validIOAddr(m,a+sizeof(Word))
  struct mem_device* d = ioAddrToDev(m, addr);
  if (d) {
    if (d->ldwfunc) d->ldwfunc(d->devptr, m, addr, v);
    return 1;
  }
  return -1;
}

int mem_map_storeDoubleWord(struct Machine* m, Address addr, DoubleWord v) {
  // we allow Word operations to actually go past io range
  // eg. load of word on last byte of io range will still
  //     invoke load word of that device mapping
  // aka not doing validIOAddr(m,a+sizeof(Word))
  struct mem_device* d = ioAddrToDev(m, addr);
  if (d) {
    if (d->swfunc) d->sdwfunc(d->devptr, m, addr, v);
    return 1;
  }
  return -1;
}

int mem_img_load(struct Machine* m, int fd) {
  int rc, n = 0;

  while (n < m->info.memSize) {
    rc = read(fd, &(m->state.memory[n]), m->info.memSize - n);
    if (rc == 0 || (rc < 0 && errno != EINTR)) break;
    n += rc;
  }
  VPRINT("n=%d fd=%d\n", n, fd);

  // postconditions
  assert(n == m->info.memSize);
  return 1;
}

int mem_img_save(struct Machine* m, int fd) {
  int rc, n = 0;

  while (n < m->info.memSize) {
    rc = write(fd, &(m->state.memory[n]), m->info.memSize - n);
    if (rc == 0 || (rc < 0 && errno != EINTR)) break;
    n += rc;
  }
  VPRINT("n=%d fd=%d\n", n, fd);

  // postconditions
  assert(n == m->info.memSize);
  return 1;
}
