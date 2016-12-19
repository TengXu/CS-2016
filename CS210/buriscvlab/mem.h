#ifndef __MEM_H__
#define __MEM_H__
#include <string.h>
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

// memory image loading and saving
int mem_img_load(struct Machine* m, int fd);
int mem_img_save(struct Machine* m, int fd);

// memory mapping interfaces
typedef void (*memdev_loadByte)(void*, struct Machine* m, Address addr,
                                Byte* v);

typedef void (*memdev_storeByte)(void*, struct Machine* m, Address addr,
                                 Byte v);

typedef void (*memdev_loadHalfWord)(void*, struct Machine* m, Address addr,
                                    HalfWord* v);

typedef void (*memdev_storeHalfWord)(void*, struct Machine* m, Address addr,
                                     HalfWord v);

typedef void (*memdev_loadWord)(void*, struct Machine* m, Address addr,
                                Word* v);

typedef void (*memdev_storeWord)(void*, struct Machine* m, Address addr,
                                 Word v);

typedef void (*memdev_loadDoubleWord)(void*, struct Machine* m, Address addr,
                                      DoubleWord* v);

typedef void (*memdev_storeDoubleWord)(void*, struct Machine* m, Address addr,
                                       DoubleWord v);

int mem_mappings_init(struct Machine* m);
int mem_map_device(struct Machine* m, Address* addr, void* devptr,
                   memdev_loadByte lbfunc, memdev_storeByte sbfunc,
                   memdev_loadHalfWord lhwfunc, memdev_storeHalfWord shwfunc,
                   memdev_loadWord lwfunc, memdev_storeWord swfunc,
                   memdev_loadDoubleWord ldwfunc,
                   memdev_storeDoubleWord sdwfunc);
int mem_unmap_device(struct Machine* m, Address addr, void** devptr);
int mem_map_loadByte(struct Machine* m, Address addr, Byte* v);
int mem_map_storeByte(struct Machine* m, Address addr, Byte v);
int mem_map_loadHalfWord(struct Machine* m, Address addr, HalfWord* v);
int mem_map_storeHalfWord(struct Machine* m, Address addr, HalfWord v);
int mem_map_loadWord(struct Machine* m, Address addr, Word* v);
int mem_map_storeWord(struct Machine* m, Address addr, Word v);
int mem_map_loadDoubleWord(struct Machine* m, Address addr, DoubleWord* v);
int mem_map_storeDoubleWord(struct Machine* m, Address addr, DoubleWord v);

// memory operators
// operators on memory
typedef enum { MEM_LOAD = 0, MEM_STORE } MemAccessType;

static inline int goodIOAddr(struct Machine* m, Address addr) {
  return (addr >= m->info.ioBaseAddr &&
          addr < (m->info.ioBaseAddr + m->info.ioLen));
}

static inline Byte mem_badAddr(struct Machine* m, Address addr, int size,
                               MemAccessType t) {
  // we could switch this to raise bad addr exception
  fprintf(stderr, "%s: addr=" PRI_ADDR " size=%d t=%d : NYI\n", __func__, addr,
          size, t);
  assert(0);
}

static inline Byte mem_getByte(struct Machine* m, Address a) {
  if (a < m->info.memSize) return m->state.memory[a];
  {
    Byte b;
    if (goodIOAddr(m, a) && mem_map_loadByte(m, a, &b)) return b;
  }
  return mem_badAddr(m, a, sizeof(Byte), MEM_LOAD);
}

static inline void mem_putByte(struct Machine* m, Address a, Byte v) {
  if (a < m->info.memSize) {
    m->state.memory[a] = v;
    return;
  }
  if (goodIOAddr(m, a) && mem_map_storeByte(m, a, v)) return;
  mem_badAddr(m, a, sizeof(Byte), MEM_STORE);
}

static inline void mem_putHalfword(struct Machine* m, Address a, HalfWord v) {
  if (a + (sizeof(HalfWord) - 1) < m->info.memSize) {
    HalfWord* hp = (HalfWord*)&(m->state.memory[a]);
    *hp = v;
    return;
  }
  if (goodIOAddr(m, a) && mem_map_storeHalfWord(m, a, v)) return;
  mem_badAddr(m, a, sizeof(HalfWord), MEM_STORE);
}

static inline HalfWord mem_getHalfword(struct Machine* m, Address a) {
  // FIXME: check alignment if we need too
  //        check edge conditions
  //        decide on how to handle endianness issues
  //        and deal with I/O
  // we could switch this to raise bad addr exception
  if (a + (sizeof(HalfWord) - 1) < m->info.memSize) {
    HalfWord* hp = (HalfWord*)&(m->state.memory[a]);
    return *hp;
  }
  {
    HalfWord hw;
    if (goodIOAddr(m, a) && mem_map_loadHalfWord(m, a, &hw)) return hw;
  }

  return mem_badAddr(m, a, sizeof(HalfWord), MEM_LOAD);
}

static inline void mem_putWord(struct Machine* m, Address a, Word v) {
  // if (a < m->info.memSize) { m->state.memory[a] = v; return; }
  if (a + (sizeof(Word) - 1) < m->info.memSize) {
    Word* wp = (Word*)&(m->state.memory[a]);
    *wp = v;
    return;
  }
  if (goodIOAddr(m, a) && mem_map_storeWord(m, a, v)) return;
  mem_badAddr(m, a, sizeof(Word), MEM_STORE);
}

static inline Word mem_getWord(struct Machine* m, Address a) {
  // FIXME: check alignment if we need too
  //        check edge conditions
  //        decide on how to handle endianness issues
  //        and deal with I/O
  // we could switch this to raise bad addr exception
  if (a + (sizeof(Word) - 1) < m->info.memSize) {
    Word* wp = (Word*)&(m->state.memory[a]);
    return *wp;
  }
  {
    Word w;
    if (goodIOAddr(m, a) && mem_map_loadWord(m, a, &w)) return w;
  }
  return mem_badAddr(m, a, sizeof(Word), MEM_LOAD);
}

static inline void mem_putDoubleWord(struct Machine* m, Address a,
                                     DoubleWord v) {
  // if (a < m->info.memSize) { m->state.memory[a] = v; return; }
  if (a + (sizeof(DoubleWord) - 1) < m->info.memSize) {
    DoubleWord* dp = (DoubleWord*)&(m->state.memory[a]);
    *dp = v;
    return;
  }
  if (goodIOAddr(m, a) && mem_map_storeDoubleWord(m, a, v)) return;
  mem_badAddr(m, a, sizeof(DoubleWord), MEM_STORE);
}

static inline DoubleWord mem_getDoubleWord(struct Machine* m, Address a) {
  // FIXME: check alignment if we need too
  //        check edge conditions
  //        decide on how to handle endianness issues
  //        and deal with I/O
  // we could switch this to raise bad addr exception
  if (a + (sizeof(DoubleWord) - 1) < m->info.memSize) {
    DoubleWord* dp = (DoubleWord*)&(m->state.memory[a]);
    return *dp;
  }
  {
    DoubleWord w;
    if (goodIOAddr(m, a) && mem_map_loadDoubleWord(m, a, &w)) return w;
  }
  return mem_badAddr(m, a, sizeof(DoubleWord), MEM_LOAD);
}
#endif
