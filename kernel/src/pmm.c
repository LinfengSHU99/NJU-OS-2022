#ifdef TEST
#include "../test/common.h"
#else
#include <common.h>
#endif

#define NUM 1000
int lock = 0;

typedef struct Node {
  uintptr_t start;
  uintptr_t end;
  int next;
  int pre;
}Node;

Node list[NUM];
int pool[NUM] = {0};
static int top = 0;

int get_start(uintptr_t start, uintptr_t end, size_t size, uintptr_t t) {
  uintptr_t s = 0;
  while (s < start) {
    s += t;
  }
  if (end - s > size) {
    return s;
  }
  return -1;
}

int get_free_id() {
  assert(top > 0);
  return pool[--top];
}

void put_id(int v) {
  assert(top <= NUM);
  pool[top++] = v;
}

static void *kalloc(size_t size) {
  assert(size < 16 * 1024 * 1024);
  while (atomic_xchg(&lock, 1) == 1) {

  }
  uintptr_t s = 0;
  for (int i = 0; i != -1; ) {
    
    int next = list[i].next;
    assert(next != -1);
    uintptr_t t = 1;
    while (t < size) {
      t = t << 1;
    }
    s = get_start(list[i].end, list[next].start, size, t);
    if (s != -1) {
      int id = get_free_id();
      list[id].next = next;
      list[id].pre = i;
      list[id].start = s;
      list[id].end = s + size;
      list[i].next = id;
      list[next].pre = id;
      break;
    }
    i = next;
  }
  atomic_xchg(&lock, 0);
  return (void*)s;
}

static void kfree(void *ptr) {
  int i = 0;

  while (atomic_xchg(&lock, 1) == 1) {

  }
  while (list[i].next != -1) {
    if ((void*)(list[i].start) == ptr) {
      put_id(i);
      int pre = list[i].pre;
      int next = list[i].next;
      list[pre].next = next;
      list[next].pre = pre;
    }
  }
  atomic_xchg(&lock, 0);
}

#ifdef TEST
static void pmm_init() {
  char *ptr  = malloc(HEAP_SIZE);
  heap.start = ptr;
  heap.end   = ptr + HEAP_SIZE;
  printf("Got %d MiB heap: [%p, %p)\n", HEAP_SIZE >> 20, heap.start, heap.end);
}
#else

static void pmm_init() {
  uintptr_t pmsize = ((uintptr_t)heap.end - (uintptr_t)heap.start);
  printf("Got %d MiB heap: [%p, %p)\n", pmsize >> 20, heap.start, heap.end);
  list[0].next = 1;
  list[0].pre = -1;
  list[0].start = (uintptr_t)(heap.start);
  list[0].end = (uintptr_t)(heap.end);
  list[1].next = -1;
  list[1].pre = 0;
  list[1].start = (uintptr_t)(heap.end);
  list[1].end = (uintptr_t)(heap.end);
  for (int i = 2; i < NUM; i++) {
    pool[top++] = i;
  }
}
#endif
MODULE_DEF(pmm) = {
  .init  = pmm_init,
  .alloc = kalloc,
  .free  = kfree,
};
