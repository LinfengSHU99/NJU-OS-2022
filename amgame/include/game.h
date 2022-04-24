#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <klib-macros.h>

typedef struct Ball {
  int x;
  int y;
}Ball;

void splash();
void print_key();
static inline void puts(const char *s) {
  for (; *s; s++) putch(*s);
}
void init_screen();
void update_screen();
void wait_key();