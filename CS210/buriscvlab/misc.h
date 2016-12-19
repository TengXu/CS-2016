#ifndef __MISC_H__
#define __MISC_H__
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
// The following block of defines control if extra code is enabled
// In various places in the code testing an tracing code can be inserted
// but only compiled and enabled if the associated define exits
//  e.g. in the inst_loop function you will find:
//       TRACE_LOOP(dump_nextInstructionState(m));
//  To turn this code on you would uncomment the ENABLE_TRACE_LOOP line below

//#define ENABLE_TRACE_LOOP
//#define ENABLE_TRACE_INIT
//#define ENABLE_TRACE_CON
//#define ENABLE_TESTS
//#define ENABLE_VERBOSE
//#define ENABLE_IPRINT
//#define ENABLE_TRACE_INT
//#define ENABLE_TRACE_INST
//#define ENABLE_TRACE_MEM
//#define ENABLE_SLEEP

// Should not have to touch what is below the behaviour should be controalled
// form commenting or uncommenting above defines

#ifdef ENABLE_VERBOSE
#define VPRINT(fmt, ...) fprintf(stderr, "%s: " fmt, __func__, __VA_ARGS__)
#define VERBOSE(stmt)                                                          \
  { stmt; }
#else
#define VPRINT(...)
#define VERBOSE(stmt)
#endif

#ifdef ENABLE_IPRINT
#define IPRINT(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define IPRINT(...)
#endif

#ifdef ENABLE_TRACE_LOOP
#define TRACE_LOOP(stmt)                                                       \
  { stmt; }
#else
#define TRACE_LOOP(stmt)
#endif

#ifdef ENABLE_TRACE_INIT
#define TRACE_INIT(stmt)                                                       \
  { stmt; }
#else
#define TRACE_INIT(stmt)
#endif

#ifdef ENABLE_TESTS
#define DO_TESTS(stmt)                                                         \
  { stmt; }
#else
#define DO_TESTS(stmt)
#endif

#ifdef ENABLE_TRACE_INT
#define TRACE_INT(stmt)                                                        \
  { stmt; }
#else
#define TRACE_INT(stmt)
#endif

#ifdef ENABLE_TRACE_INST
#define TRACE_INST(stmt)                                                       \
  { stmt; }
#else
#define TRACE_INST(stmt)
#endif

#ifdef ENABLE_TRACE_MEM
#define TRACE_MEM(stmt)                                                        \
  { stmt; }
#else
#define TRACE_MEM(stmt)
#endif

#ifdef ENABLE_TRACE_CON
#define TRACE_CON(stmt)                                                        \
  { stmt; }
#else
#define TRACE_CON(stmt)
#endif

#define NYI fprintf(stderr, "%s: NYI\n", __func__)

enum DumpMemDirective { MEM_NONE, MEM_ALL, MEM_SPECIAL };
void dump_machine(struct Machine* m, enum DumpMemDirective md);
void dump_cpu(struct Machine* m);
void dump_regfile(struct RegisterFile* rf);
void dump_mem(struct Machine* m, Address addr, Word len);
void dump_gprs(struct RegisterFile* rf);
void dump_nextInstructionState(struct Machine* m);
#endif
