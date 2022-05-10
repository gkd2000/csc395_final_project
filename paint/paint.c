#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>
#include <ugraphics.h>

typedef struct mouse_data {
    int32_t x_pos;
    int32_t y_pos;
    bool left_click;
    bool right_click;
    bool middle_click;
} mouse_data_t;

int _start() {

  mouse_data_t* user_mouse = malloc(sizeof(mouse_data_t));

  while(true) {
    readmouse(user_mouse);

    if(user_mouse->left_click) {
      for (int i = 0; i < 300; i++) {
        drawpixel(608, 608 + i, 123, 200, 189);
      }
    }
  }

  return 0;
}