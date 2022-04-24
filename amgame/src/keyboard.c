#include <game.h>

#define KEYNAME(key) \
  [AM_KEY_##key] = #key,
static const char *key_names[] = {
  AM_KEYS(KEYNAME)
};

void print_key() {
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  ioe_read(AM_INPUT_KEYBRD, &event);
  if (event.keycode != AM_KEY_NONE && event.keydown) {
    puts("Key pressed: ");
    puts(key_names[event.keycode]);
    puts("\n");
  }
}

extern Ball ball;
extern int w, h;

void wait_key() {
  // int keycode = AM_KEY_NONE;
  // int key_status = 0;
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  while (event.keycode == AM_KEY_NONE) {
    // putstr("reach here\n");
    // keycode = io_read(AM_INPUT_KEYBRD).keycode;
    // key_status = io_read(AM_INPUT_KEYBRD).keydown; 
    ioe_read(AM_INPUT_KEYBRD, &event);
    // printf("%d\n", event.keycode);
    if (event.keydown == 1) {
      if (event.keycode == AM_KEY_ESCAPE) {
        halt(0);
      }
      switch (event.keycode) {
        case AM_KEY_DOWN: ball.y = ball.y + 1 < h - 1 ? ball.y + 1 : ball.y; break;
        case AM_KEY_UP: ball.y = ball.y - 1 > 0 ? ball.y - 1 : ball.y; break;
        case AM_KEY_RIGHT: ball.x = ball.x + 1 < w - 1 ? ball.x + 1 : ball.x; break;
        case AM_KEY_LEFT: ball.x = ball.x - 1 > 0 ? ball.x - 1 : ball.x; break;
      } 
    }
  }
}
