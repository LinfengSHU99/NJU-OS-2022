#include "thread.h"
// #include <kernel.h>
#include "common.h"
// extern 
static void entry(int tid) { 
  void *p = pmm->alloc(128);
  printf("%p\n", p);
  sleep(1);
  pmm->free(p);
 }
static void goodbye()      { printf("End.\n"); }

int main() {
  pmm->init();
  for (int i = 0; i < 60; i++)
    create(entry);
  join();
  goodbye();
  // pmm->alloc(128);
}