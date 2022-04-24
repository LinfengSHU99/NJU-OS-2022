#include <game.h>

// Operating system is a C program!
Ball ball;
int main(const char *args) {
  ioe_init();

  puts("mainargs = \"");
  puts(args); // make run mainargs=xxx
  puts("\"\n");

  // splash();

  // puts("Press any key to see its key code...\n");
  init_screen();
  while (1) {
    // print_key();
    wait_key();
    update_screen();

  }
  return 0;
}
