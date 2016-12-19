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

static void test_signExtendToRegister(void) {
  Register y;

  printf("%s: signExtendToRegister tests: BEGIN\n", __func__);
  signed char c = -1;
  unsigned char uc = -1;
  signExtendToRegister(c, y);
  printf("c=0x%02hhx y=0x%016lx\n", c, y);
  assert(y == 0xffffffffffffffffUL);
  signExtendToRegister(uc, y);
  printf("uc=0x%02hhx y=0x%016lx\n", uc, y);
  assert(y == 0xffffffffffffffffUL);
  c = 1;
  uc = 1;
  signExtendToRegister(c, y);
  printf("c=0x%02hhx y=0x%016lx\n", c, y);
  assert(y == 0x0000000000000001UL);
  signExtendToRegister(uc, y);
  printf("uc=0x%02hhx y=0x%016lx\n", uc, y);
  assert(y == 0x0000000000000001UL);
  c = 0;
  uc = 0;
  signExtendToRegister(c, y);
  printf("c=0x%02hhx y=0x%016lx\n", c, y);
  assert(y == 0x0UL);
  signExtendToRegister(uc, y);
  printf("uc=0x%02hhx y=0x%016lx\n", uc, y);
  assert(y == 0x0UL);

  signed short s = -1;
  unsigned short us = -1;
  signExtendToRegister(s, y);
  printf("s=0x%04hx y=0x%016lx\n", s, y);
  assert(y == 0xffffffffffffffffUL);
  signExtendToRegister(us, y);
  printf("sc=0x%04hx y=0x%016lx\n", us, y);
  assert(y == 0xffffffffffffffffUL);
  s = 1;
  us = 1;
  signExtendToRegister(s, y);
  printf("s=0x%04hx y=0x%016lx\n", s, y);
  assert(y == 0x0000000000000001UL);
  signExtendToRegister(us, y);
  printf("sc=0x%04hx y=0x%016lx\n", us, y);
  assert(y == 0x0000000000000001UL);
  s = 0;
  us = 0;
  signExtendToRegister(s, y);
  printf("s=0x%04hx y=0x%016lx\n", s, y);
  assert(y == 0UL);
  signExtendToRegister(us, y);
  printf("sc=0x%04hx y=0x%016lx\n", us, y);
  assert(y == 0UL);

  signed int i = -1;
  unsigned int ui = -1;
  signExtendToRegister(i, y);
  printf("i=0x%08x y=0x%016lx\n", i, y);
  assert(y == 0xffffffffffffffffUL);
  assert(y = -1);
  signExtendToRegister(ui, y);
  printf("ui=0x%08x y=0x%016lx\n", ui, y);
  assert(y == 0xffffffffffffffffUL);
  i = 1;
  ui = 1;
  signExtendToRegister(i, y);
  printf("i=0x%08x y=0x%016lx\n", i, y);
  assert(y == 0x0000000000000001UL);
  signExtendToRegister(ui, y);
  printf("ui=0x%08x y=0x%016lx\n", ui, y);
  assert(y == 0x0000000000000001UL);
  i = 0;
  ui = 0;
  signExtendToRegister(i, y);
  printf("i=0x%08x y=0x%016lx\n", i, y);
  assert(y == 0x0UL);
  signExtendToRegister(ui, y);
  printf("ui=0x%08x y=0x%016lx\n", ui, y);
  assert(y == 0x0UL);

  signed long l = -1;
  unsigned long ul = -1;
  signExtendToRegister(l, y);
  printf("l=0x%016lx y=0x%016lx\n", l, y);
  assert(y == 0xffffffffffffffffUL);
  signExtendToRegister(ul, y);
  printf("ul=0x%016lx y=0x%016lx\n", ul, y);
  assert(y == 0xffffffffffffffffUL);
  l = 1;
  ul = 1;
  signExtendToRegister(l, y);
  printf("l=0x%016lx y=0x%016lx\n", l, y);
  assert(y == 0x0000000000000001UL);
  signExtendToRegister(ul, y);
  printf("ul=0x%016lx y=0x%016lx\n", ul, y);
  assert(y == 0x0000000000000001UL);
  l = 0;
  ul = 0;
  signExtendToRegister(l, y);
  printf("l=0x%016lx y=0x%016lx\n", l, y);
  assert(y == 0x0UL);
  signExtendToRegister(ul, y);
  printf("ul=0x%016lx y=0x%016lx\n", ul, y);
  assert(y == 0x0UL);

  printf("%s: signExtendToRegister tests: END\n", __func__);
}

void test_zeroExtendToRegister(void) {
  Register y;

  printf("%s: zeroExtendToRegister tests: BEGIN\n", __func__);
  signed char c = -1;
  unsigned char uc = -1;
  zeroExtendToRegister(c, y);
  printf("c=0x%02hhx y=0x%016lx\n", c, y);
  assert(y == 0xffUL);
  zeroExtendToRegister(uc, y);
  printf("uc=0x%02hhx y=0x%016lx\n", uc, y);
  assert(y == 0xffUL);
  c = 1;
  uc = 1;
  zeroExtendToRegister(c, y);
  printf("c=0x%02hhx y=0x%016lx\n", c, y);
  assert(y == 0x0000000000000001UL);
  zeroExtendToRegister(uc, y);
  printf("uc=0x%02hhx y=0x%016lx\n", uc, y);
  assert(y == 0x0000000000000001UL);
  c = 0;
  uc = 0;
  zeroExtendToRegister(c, y);
  printf("c=0x%02hhx y=0x%016lx\n", c, y);
  assert(y == 0x0UL);
  zeroExtendToRegister(uc, y);
  printf("uc=0x%02hhx y=0x%016lx\n", uc, y);
  assert(y == 0x0UL);

  signed short s = -1;
  unsigned short us = -1;
  zeroExtendToRegister(s, y);
  printf("s=0x%04hx y=0x%016lx\n", s, y);
  assert(y == 0xffffUL);
  zeroExtendToRegister(us, y);
  printf("sc=0x%04hx y=0x%016lx\n", us, y);
  assert(y == 0xffffUL);
  s = 1;
  us = 1;
  zeroExtendToRegister(s, y);
  printf("s=0x%04hx y=0x%016lx\n", s, y);
  assert(y == 0x0000000000000001UL);
  zeroExtendToRegister(us, y);
  printf("sc=0x%04hx y=0x%016lx\n", us, y);
  assert(y == 0x0000000000000001UL);
  s = 0;
  us = 0;
  zeroExtendToRegister(s, y);
  printf("s=0x%04hx y=0x%016lx\n", s, y);
  assert(y == 0UL);
  zeroExtendToRegister(us, y);
  printf("sc=0x%04hx y=0x%016lx\n", us, y);
  assert(y == 0UL);

  signed int i = -1;
  unsigned int ui = -1;
  zeroExtendToRegister(i, y);
  printf("i=0x%08x y=0x%016lx\n", i, y);
  assert(y == 0xffffffffUL);
  assert(y = -1);
  zeroExtendToRegister(ui, y);
  printf("ui=0x%08x y=0x%016lx\n", ui, y);
  assert(y == 0xffffffffUL);
  i = 1;
  ui = 1;
  zeroExtendToRegister(i, y);
  printf("i=0x%08x y=0x%016lx\n", i, y);
  assert(y == 0x0000000000000001UL);
  zeroExtendToRegister(ui, y);
  printf("ui=0x%08x y=0x%016lx\n", ui, y);
  assert(y == 0x0000000000000001UL);
  i = 0;
  ui = 0;
  zeroExtendToRegister(i, y);
  printf("i=0x%08x y=0x%016lx\n", i, y);
  assert(y == 0x0UL);
  zeroExtendToRegister(ui, y);
  printf("ui=0x%08x y=0x%016lx\n", ui, y);
  assert(y == 0x0UL);

  signed long l = -1;
  unsigned long ul = -1;
  zeroExtendToRegister(l, y);
  printf("l=0x%016lx y=0x%016lx\n", l, y);
  assert(y == 0xffffffffffffffffUL);
  zeroExtendToRegister(ul, y);
  printf("ul=0x%016lx y=0x%016lx\n", ul, y);
  assert(y == 0xffffffffffffffffUL);
  l = 1;
  ul = 1;
  zeroExtendToRegister(l, y);
  printf("l=0x%016lx y=0x%016lx\n", l, y);
  assert(y == 0x0000000000000001UL);
  zeroExtendToRegister(ul, y);
  printf("ul=0x%016lx y=0x%016lx\n", ul, y);
  assert(y == 0x0000000000000001UL);
  l = 0;
  ul = 0;
  zeroExtendToRegister(l, y);
  printf("l=0x%016lx y=0x%016lx\n", l, y);
  assert(y == 0x0UL);
  zeroExtendToRegister(ul, y);
  printf("ul=0x%016lx y=0x%016lx\n", ul, y);
  assert(y == 0x0UL);

  printf("%s: zeroExtendToRegister tests: END\n", __func__);
}

void extension_tests(void) {
  test_signExtendToRegister();
  test_zeroExtendToRegister();
}

void test_runAllTests(void) {
  // Extension tests
  extension_tests();
}
