// test-tmalloc.cpp
//
// Test functions for tmalloc.cpp

/*
  Copyright (c) 2015 VMware, Inc
  All rights reserved. 

  Written by Marcos K. Aguilera

  MIT License

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the ""Software""), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "inttypes.h"
#include "prng.h"
#include "tmalloc.h"

#define NBUFS 64
#define BUFSIZE 32

// checks that buf of size len is filled with c
// len must be a multiple of 8
int chkbuf(void *buf, int len, int c){
  int i;
  u64 pattern;
  u64 *ptr64;
  assert(len%8==0);
  len /= 8;
  c &= 255;
  for (i=0; i < 8; ++i){
    pattern <<= 8;
    pattern |= c;
  }
  ptr64 = (u64*) buf;
  for (i=0; i < len; ++i){
    if (*ptr64 != pattern) break;
    ++ptr64;
  }
  if (i < len) return 0;
  else return 1;
}

#define TestTmalloc_NOPSK 10000
void TestTmalloc(){
  Prng rnd(1);
  int i, j;
  void *bufs[NBUFS];
  int n;
  int res;
  
  for (i=0; i < NBUFS; ++i) bufs[i]=0;
  
  printf("Testing tmalloc\n");
  for (i=0; i < TestTmalloc_NOPSK*1000; ++i){
    n = (unsigned) rnd.next() % NBUFS;
    if (bufs[n]){
      free(bufs[n]);
      bufs[n] = 0;
    } else {
      bufs[n] = malloc(BUFSIZE);
    }
    for (j=0; j < NBUFS; ++j){
      if (bufs[j]) memset(bufs[j], j, BUFSIZE);
    }
    for (n=j=0; j < NBUFS; ++j){
      if (bufs[j]){
        ++n;
        res = chkbuf(bufs[j], BUFSIZE, j);
        assert(res);
      }
    }
    
    if (i % 100000 == 0){
      printf(" %d iterations (%d bufs)\n", i, n);
      fflush(stdout);
    }
  }
}

int main(int argc, char **argv) {
  TestTmalloc();
  return 0;
}
