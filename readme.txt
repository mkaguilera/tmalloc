tmalloc is a malloc replacement that provides thread-local allocation for better efficiency.

To use, simply include

  #include "tmalloc.h"
  
in your source file and then link your program with tmalloc.o

For an example, see example.cpp. The makefile will build it.

Note: the code has not been tested well in Windows
