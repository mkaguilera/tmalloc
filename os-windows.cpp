// os-windows.cpp
//
// OS specific stuff for Windows

/*
  Original code: Copyright (c) 2014 Microsoft Corporation
  Modified code: Copyright (c) 2015 VMware, Inc
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

#include "os-windows.h"

// create a new thread starting func(arg)
// Sets *thread to thread id
// Returns 0 if ok, non-zero if error
int OSCreateThread(OSThread_t *thread, OSThread_return_t (*func)(void*), void *arg){
  HANDLE res;
  res = CreateThread(0, 0, func, arg, 0, 0);
  if (res){
    *thread = res;
    return 0;
  }
  else return -1;
}

// wait for thread to end, storing its return value in *res
// Returns 0 if ok, non-zero if error
int OSWaitThread(OSThread_t thread, void **res){
  int res;
  DWORD exitcode;
  res = WaitForSingleObject(thread, INFINITE);
  res = GetExitCodeThread(thread, &exitcode); assert(res);
  *res = (void) exitcode;
}
