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
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "common.h"
#include "console.h"
#include "team.h"
#include "test.h"

int main(int argc, char** argv) {
  int rc;
  int n = 0;
  int inmemfd, outmemfd;
  struct Machine* m;

  DO_TESTS({
    test_runAllTests();
    return -1;
  });

  if (argc < 3) {
    printf("RISCVLab: teamname=%s m0=%s:%s m1=%s:%s\n", team.teamname,
           team.name1, team.id1, team.name2, team.id2);
    fprintf(stderr, "USAGE: %s <inimgfile> <outimgfile> [count]\n", argv[0]);
    return -1;
  }

  inmemfd = open(argv[1], O_RDONLY);
  if (inmemfd < 0) {
    fprintf(stderr, "ERROR opening %s\n", argv[1]);
    return (-1);
  }

  outmemfd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
  if (outmemfd < 0) {
    fprintf(stderr, "ERROR opening %s\n", argv[2]);
    return (-1);
  }

  if (argc == 4) n = atoi(argv[3]);

  // create and initialize the machine structure
  rc = buriscv_machineInit(&m, inmemfd);
  if (rc < 0) return (-1);

  // load memory image
  rc = mem_img_load(m, inmemfd);
  if (rc < 0) return (-1);

  // initialize memory mapping infrastructure
  rc = mem_mappings_init(m);
  if (rc < 0) return (-1);

  // initialize console
  rc = console_init(m);
  if (rc < 0) return (-1);

  // initialize interrupt subsystem
  int_init(m);

  // invoke reset interrupt logic
  rc = int_reset(m);
  if (rc == 0) return (-1);

  TRACE_INIT({
    fprintf(stderr, "INITIAL STATE:\n");
    dump_machine(m, MEM_SPECIAL);
    fprintf(stderr, "-------------\n\n");
  });

  rc = inst_loop(m, n);

  rc = mem_img_save(m, outmemfd);

  close(outmemfd);

  return 0;
}
