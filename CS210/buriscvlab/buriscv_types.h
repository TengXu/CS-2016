#ifndef __BURISCV_TYPES_H__
#define __BURISCV_TYPES_H__
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

typedef uint64_t DoubleWord;
typedef uint32_t Word;
typedef uint16_t HalfWord;
typedef uint8_t Byte;
typedef int64_t SDoubleWord;
typedef int32_t SWord;
typedef int16_t SHalfWord;
typedef int8_t SByte;

#define XLEN 64
typedef DoubleWord Register;
typedef SDoubleWord SRegister;
typedef DoubleWord Address;
#define PRI_REG "%016lx"
#define PRI_ADDR "%016lx"
#define PRI_NUM "%ld"
#define PRI_UNUM "%lu"
#endif
