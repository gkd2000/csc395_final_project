#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>
#include <ugraphics.h>

// Number of colors in the palette at the top of the screen
#define NUM_COLORS 41 

// Structure to store processed mouse data, identical to the kernel's structure for the same purpose
typedef struct mouse_data {
    int32_t x_pos;
    int32_t y_pos;
    bool left_click;
    bool right_click;
    bool middle_click;
} mouse_data_t;

// Display all of the colors that we have available (hardcoded in the color_values array)
void color_selection(uint32_t* color_values) {
  // The current color display will NOT be updated in this function, as it is updated at different times

  for(int i = 0; i < NUM_COLORS; i++) {
    draw_rectangle(80 + i * 20, 25, 15, 50,  color_values[i]);
  }
}

// Set up the top part of the screen where the user can see the current color, pick new
// colors, and clear the screen
void setup_color_selection_pane(uint32_t active_color, uint32_t* color_values) {
  // Draw the GRADIENT (:
  uint32_t gradient_color = 0x707570;
  for(int i = 0; i < SCREEN_WIDTH+1; i += 13) {
    draw_rectangle(i, 0, 13, 97, gradient_color);
    gradient_color += 0x010101;
  }

  // Draw separator for color selector pane
  draw_rectangle(0, 97, SCREEN_WIDTH+1, 3, WHITE);

  // Draw clear screen button
  draw_rectangle(900, 25, 80, 50, 0x000000);
  gwrite(920, 46, 0xC0C0C0, "CLEAR", 5);

  // Draw the color selection rectangles
  color_selection(color_values);

  // Draw the current/active color
  draw_rectangle(25, 25, 50, 40, active_color);

  // Write hexadecimal representation of the current color
  char* active_color_chars = malloc(sizeof(char) * 20);
  gwrite(26, 67, WHITE, itoa_x(active_color, active_color_chars, 6), 6);
}

void _start() {

  // 
  uint32_t color_values[NUM_COLORS] = {0x800000, 0xFF0000, 0xFF8080, 0x804000, 0xFF8000, 0xFFC000, 0x808000, 0xFFFF00, 0xFFFF80, // red, orange, and yellow
                                       0x408000, 0x80FF00, 0xC0FF80, 0x008000, 0x00FF00, 0x80FF80, 0x008040, 0x00FF80, 0x80FFC0, // greens
                                       0x008080, 0x00FFFF, 0x80FFFF, 0x004080, 0x0080FF, 0x80C0FF, 0x000080, 0x0000FF, 0x8080FF, // blues
                                       0x400080, 0x8000FF, 0xC080FF, 0x800080, 0xFF00FF, 0xFF80FF, 0x800040, 0xFF0080, 0xFF80C0, // purple, magenta, pink
                                       0x000000, 0x404040, 0x808080, 0xC0C0C0, 0xFFFFFF}; // white, grey, black

  // Make space for the data from the mouse
  mouse_data_t* user_mouse = malloc(sizeof(mouse_data_t));
  if (user_mouse == NULL) {
    exit(1);
  }

  // Read a keypress (if there is one to read)
  char arr[1];
  read(STDIN_NONBLOCKING, arr, 1);

  // Initialize active color to red
  uint32_t active_color = 0xFF0000;

  // Draw the color palette
  setup_color_selection_pane(active_color, color_values);

  // Loop until the user presses q
  while(arr[0] != 'q' && arr[0] != 'Q') {

    // Get mouse data
    readmouse((uintptr_t) user_mouse);

    // Check if the mouse is clicked
    if(user_mouse->left_click && user_mouse->y_pos > 100) {
      // We're on the canvas. Draw behind the cursor in the active color
      update_cursor_background(active_color);
    } else if(user_mouse->left_click && user_mouse->y_pos <= 75 && user_mouse->y_pos >= 25) { 
      //  We are in the color selection area, so select a color!

      // Compute the index of the color array corresponding to where the click occurred 
      int potential_index = (user_mouse->x_pos - 80) / 20;

      // If the proposed index is within bounds of the array, switch to the appropriate color 
      if(potential_index >= 0 && potential_index < NUM_COLORS) {
        // Set the active color to the value within the array
        active_color = color_values[potential_index];
        draw_rectangle(25, 25, 50, 40, active_color);

        // Write hexadecimal representation
        char* active_color_chars = malloc(sizeof(char) * 20);
        gwrite(25, 67, WHITE, itoa_x(active_color, active_color_chars, 6), 6);
      }

      // If the mouse is on the clear button, clear the drawing space and re-draw the color selector
      if(user_mouse->x_pos >= 900 && user_mouse->x_pos <= 980) {
        draw_rectangle(0, 100, SCREEN_WIDTH, SCREEN_HEIGHT - 100, 0x000000);

        setup_color_selection_pane(active_color, color_values);
      }
    }

    // Check for keyboard input
    read(STDIN_NONBLOCKING, arr, 1);
    
  }

  // We have read a 'q' keypress from the user. Exit.
  exit(0);
}