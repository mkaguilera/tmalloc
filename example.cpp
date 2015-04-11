#include <stdio.h>
#include "tmalloc.h"

int main(int argc, char **argv){
  char *buf;
  buf = (char*) malloc(32);
  free(buf);
}

  
