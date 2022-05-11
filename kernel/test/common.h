/* common.h for test*/

// #include <kernel.h>
// #include <klib.h>
// #include <klib-macros.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <kernel.h>
#include <assert.h>
#include <stdatomic.h>

// #include <klib.h>
// #include <klib-macros.h>

#define HEAP_SIZE 134217728

typedef struct {
  void *start, *end;
} Area;

Area heap;
int atomic_xchg(int *addr, int newval) {
  return atomic_exchange((int *)addr, newval);
}
// #define MODULE(mod) \
//   typedef struct mod_##mod##_t mod_##mod##_t; \
//   extern mod_##mod##_t *mod; \
//   struct mod_##mod##_t

// #define MODULE_DEF(mod) \
//   extern mod_##mod##_t __##mod##_obj; \
//   mod_##mod##_t *mod = &__##mod##_obj; \
//   mod_##mod##_t __##mod##_obj
// MODULE(pmm) {
//   void  (*init)();
//   void *(*alloc)(size_t size);
//   void  (*free)(void *ptr);
// };

