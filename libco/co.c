#include "co.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
//#include <stddef.h>
// #include <unistd.h>

#define MAX_NUM 1000
#define NOT_RUNNING 0
#define RUNNING 1
#define STACK_SIZE 64000

extern int main();

typedef struct co {
  void(*entry)(void*);
  int id;
  char name[100];
  void *arg;
  jmp_buf buf;
  int mode;
  u_int8_t stack[STACK_SIZE];
}co;

typedef struct Node {
  co* co;
  struct Node* next;
}Node;

Node* head;
Node* tail;
co *co_main;
co *cur_co;
int cur_num = 0;
int size = 0;

__attribute__((constructor)) void init() {
  head = (Node*)malloc(sizeof(Node));
  head->co = NULL;
  head->next = NULL;
  tail = head;
  co_main = co_start("main", (void*)main, NULL);
  cur_co = co_main;
  co_main->mode = RUNNING;
  srand(666);
}

static inline void stack_switch_call(void *sp, void *entry, void* arg) {
    asm volatile (
//#if __x86_64__
    "movq %0, %%esp; movq %2, %%edi; jmp *%1"
    : : "b"((void*)sp), "d"(entry), "a"(arg) : "memory"
//#else
//    "movl %0, %%esp; movl %2, 4(%0); jmp *%1"
//      : : "b"((u_int*)sp - 8), "d"(entry), "a"(arg) : "memory"
//#endif
    );
}

void remove_co(int id) {
  Node* p = head;
  while (p->next != NULL) {
    if (p->next->co->id == id) {
      free(p->next->co);
      Node* t = p->next;
      p->next = p->next->next;
      free(t);
    }
    else {
        p = p->next;
    }
  }
  size--;
}


co *random_chose() {
  int index = (rand() % size) + 1;
  printf("%d\n", index);
  Node* p = head;
  while (index--) {
    p = p->next;
  }
  return p->co;
}

struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  assert(cur_num < MAX_NUM);
  co* co_ptr = (co*)malloc(sizeof(co));
  memset(co_ptr->stack, 0, STACK_SIZE);
  co_ptr->entry = func;
  co_ptr->id = cur_num;
  strcpy(co_ptr->name, name);
  co_ptr->arg = arg;
  co_ptr->mode = NOT_RUNNING;
  Node* node = (Node*)malloc(sizeof(Node));
  node->co = co_ptr;
  node->next = NULL;
  tail->next = node;
  tail = node;
  cur_num++;
  size++;
  return co_ptr;
}

void co_wait(struct co *co) {
  
  cur_co = co;

  co->mode = RUNNING;
  if (co != co_main){
      stack_switch_call(co->stack, co->entry, co->arg);
//    co->entry(co->arg);
      remove_co(co->id);
  }
  else {
    return;
  }
}

void co_yield() {
  if (cur_co == co_main) return;
  int r = setjmp(cur_co->buf);
  if (r == 0) {
    co *next_co = random_chose();
    if (next_co == co_main) next_co = cur_co;
    if (next_co->mode == RUNNING) longjmp(next_co->buf, 1);
    else if(next_co->mode == NOT_RUNNING) {
      co_wait(next_co);
    }
  }
  else if (r == 1) return;
  
}
