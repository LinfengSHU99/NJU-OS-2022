#include <game.h>

#define SIDE 16
int w, h;

static void init() {
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;
}

static void draw_tile(int x, int y, int w, int h, uint32_t color) {
  uint32_t pixels[w * h]; // WARNING: large stack-allocated memory
  AM_GPU_FBDRAW_T event = {
    .x = x, .y = y, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = color;
  }
  ioe_write(AM_GPU_FBDRAW, &event);
}

void splash() {
  init();
  for (int x = 0; x * SIDE <= w; x ++) {
    for (int y = 0; y * SIDE <= h; y++) {
      if ((x & 1) ^ (y & 1)) {
        draw_tile(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
      }
    }
  }
}

extern Ball ball;

void draw_ball(uint32_t pixels[]) {
  int center = ball.y * w + ball.x;
  int cnt = 0;
  for (int i = center - w - 1; cnt < 9; i++) {
    if (cnt % 3 == 0 && cnt != 0) {
      i = i + w - 3;
    }
    pixels[i] = 0xffffff;
    cnt++;
  }
}

void init_screen() {
  init();
  ball.x = w / 2;
  ball.y = h / 2;
  uint32_t pixels[w * h];
  AM_GPU_FBDRAW_T event = {
    .x = 0, .y = 0, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = 0x000000;
  }
  draw_ball(pixels);
  ioe_write(AM_GPU_FBDRAW, &event);

}

void update_screen() {
  uint32_t pixels[w * h];
  AM_GPU_FBDRAW_T event = {
    .x = 0, .y = 0, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = 0x000000;
  }
  draw_ball(pixels);
  ioe_write(AM_GPU_FBDRAW, &event);
}
