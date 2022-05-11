#include "thread.h"
// #include <kernel.h>
#include "common.h"
// extern 
static void entry(int tid) { pmm->alloc(128); }
static void goodbye()      { printf("End.\n"); }

int main() {
  pmm->init();
  for (int i = 0; i < 1000; i++)
    create(entry);
  join();
  goodbye();
  // pmm->alloc(128);
}