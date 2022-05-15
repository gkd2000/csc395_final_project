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

void _start() {

  // Make space for the data from the mouse
  mouse_data_t* user_mouse = malloc(sizeof(mouse_data_t));
  if (user_mouse == NULL) {
    exit(1);
  }

  // Read a keypress (if there is one to read)
  char arr[1];
  read(STDIN_NONBLOCKING, arr, 1);

  // Loop until the user presses q
  while(arr[0] != 'q') {

    // Get mouse data
    readmouse((uintptr_t) user_mouse);

    // If the mouse is clicked, draw behind the cursor (currently in red but maybe we can support other colors soon)
    if(user_mouse->left_click) {
      update_cursor_background(0xFF0000);
    }

    // Check for keyboard input
    read(STDIN_NONBLOCKING, arr, 1);
    
  }

  exit(0);
}