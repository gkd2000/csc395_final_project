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
  if (user_mouse == NULL) {
    for (int i = 0; i < 300; i++) {
        drawpixel(708, 708 + i, 34, 3, 200);
      // }
    }
  }

  while(true) {
    readmouse((uintptr_t) user_mouse);
    // char buf[5];
    // read(0, buf, 1);

    if(user_mouse->left_click) {
      // for (int i = 0; i < 300; i++) {
      //   drawpixel(608, 608 + i, 123, 200, 189);
      // }
      // for (int i = 0; i < 100000; i++);

      // for (int i = 0; i < 300; i++) {
      //   drawpixel(608, 608 + i, 0, 0, 0);
      // }

      // for (int i = 0; i < 100000; i++);
      update_cursor_background(0xFF0000);
    }
    // for (int i = 0; i < 5; i++) {
    //   for (int j = 0; j < 5; j++) {
    //     drawpixel(user_mouse->x_pos + i, user_mouse->y_pos + j, 255, 0, 0);
    //   }
    // }
    
  }
  return 0;
}