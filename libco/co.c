#include "co.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <stdint.h>

#define MAX_NUM 1000
#define NOT_RUNNING 0
#define RUNNING 1
#define DEAD 2
#define STACK_SIZE 64000
#define SIZE STACK_SIZE + 16

extern int main();

typedef struct co {
  void(*entry)(void*);
  int id;
  char name[100];
  void *arg;
  jmp_buf buf;
  int mode;
  uint8_t stack[SIZE];
  uintptr_t waiter_sp;
//  struct co* waiter;
}co;

// 保存已有协程的列表
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
uintptr_t sp_stack[SIZE];
int top = 0;
int cnt = 1;

// main函数启动前的准备工作
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

static inline void stack_switch_call(void *sp, void *entry, uintptr_t arg) {
    asm volatile (
#if __x86_64__
//    "movq %0, %%rsp; movq %2, %%rdi; jmp *%1"
//    : : "b"((uintptr_t )sp), "d"(entry), "a"(arg) : "memory"
    "movq %0, %%rsp;"
    : : "b"((uintptr_t )sp), "d"(entry), "a"(arg) : "memory"
#else
    "movl %0, %%esp; movl %2, 4(%0); jmp *%1"
      : : "b"((uintptr_t)sp - 8), "d"(entry), "a"(arg) : "memory"
#endif
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
  if (index == 1) index++;
//    int index = cnt % 4 + 2;
//    cnt++;
//  printf("%d\n", index);
  Node* p = head;
  while (index--) {
    p = p->next;
  }
//  printf("%s\n", p->co->name);
  return p->co;
}

struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  assert(cur_num < MAX_NUM);
  co* co_ptr = (co*)malloc(sizeof(co));
  memset(co_ptr->stack, 0, SIZE);
  co_ptr->entry = func;
  co_ptr->id = cur_num;
  co_ptr->waiter_sp = 0;
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

// 返回对齐后的栈地址，注意是stack[]数组的最后一个元素地址。(栈分配从地址空间的最大值开始往下递减)
void* get_sp(co *co) {
#if __x86_64__
    uint64_t offset = (uintptr_t) (co->stack + STACK_SIZE) % 16;
#else
    uint32_t offset = (uint32_t)co->stack % 16;
#endif
    void* sp = co->stack + STACK_SIZE;
    return sp + 16 - offset;
}

// 得到现在的$rsp
static inline uintptr_t get_rsp() {
    uintptr_t sp = 0;
#if __x86_64__
    asm volatile("movq %%rsp, %0;" : "=r"(sp) : :);
#else
#endif
    return sp;
}

// 将sp设置为$rsp
static inline void set_rsp(uintptr_t sp) {
#if __x86_64__
    asm volatile("movq %0, %%rsp;" : : "r"(sp) :);
#else
#endif
}

void co_wait(struct co *co) {
  if (co->mode == DEAD) {
      remove_co(co->id);
      return;
  }
  cur_co = co;
  if (co->mode == RUNNING) {
      longjmp(co->buf, 1);
  }
  else if (co->mode == NOT_RUNNING) {
      co->mode = RUNNING;
      if (co != co_main){
          void *sp = get_sp(co);
        co->waiter_sp = get_rsp();
        stack_switch_call(sp, co->entry, (uintptr_t )co->arg);
        co->entry(cur_co->arg);
        co->mode = DEAD;
        set_rsp((uintptr_t)co->waiter_sp);
      }
  }
    remove_co(co->id);
}

void co_yield() {
  if (cur_co == co_main) return;
  int r = setjmp(cur_co->buf);
  if (r == 0) {
    co *next_co = random_chose();
    if (next_co == co_main) next_co = cur_co;
    if (next_co->mode == RUNNING) {
        cur_co = next_co;
        longjmp(next_co->buf, 1);
    }
    
    // 注意使用next_co而不是cur_co
    else if(next_co->mode == NOT_RUNNING) {
        cur_co = next_co;
        next_co->mode = RUNNING;
        void *sp = get_sp(next_co);
        next_co->waiter_sp = get_rsp();
        stack_switch_call(sp, next_co->entry, (uintptr_t )next_co->arg);
        next_co->entry(cur_co->arg);
        next_co->mode = DEAD;
        set_rsp((uintptr_t) next_co->waiter_sp);
    }
  }
  else if (r == 1) return;
}
