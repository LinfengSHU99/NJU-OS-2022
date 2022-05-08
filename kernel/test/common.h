/* common.h for test*/

#include <kernel.h>
// #include <klib.h>
// #include <klib-macros.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define HEAP_SIZE 134217728

typedef struct {
  void *start, *end;
} Area;

Area heap;