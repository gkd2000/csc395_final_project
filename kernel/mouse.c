#include "mouse.h"

int mouse_counter; //> Keep track of which byte in the 3-byte sequence we are receiving from the mouse

// Array for the pixels that were most recently overwritten to draw the cursor. One index per pixel, with
// colors stored in hexadecimal
uint32_t saved_pixels[CURSOR_HEIGHT * CURSOR_WIDTH]; 

// Mouse initialization code from https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Wait for a particular state of the mouse by polling port 0x64. 
 * \param type Determines the bit we're waiting on and the value we're waiting for it to be
 *             If type = READY_TO_READ = 0, we're waiting until there is data available to be read on port 0x60
 *             If type = READY_TO_SEND = 1, then the mouse is ready to receive data or a command (on port 0x60 or 0x64)
 */
void mouse_wait(unsigned char type) {
  // How long do we wait before we give up?
  int time_out = 100000;

  if (type == READY_TO_READ) {
    // Wait for the least significant bit returned by the mouse to be set:
    // it is set if there is data available to be read on port 0x60
    while (time_out--) {
      if ((inb(0x64) & 1) == 1)
        return;
    }
    return;
  } else {
    // Wait for the second-least significant bit returned by the mouse to be turned off:
    // indicates that the mouse is ready to receive data or a command (either on port 0x60 or port 0x64)
    while (time_out--) {
      if ((inb(0x64) & 2) == 0)
        return;
    } 
    return;
  }
}

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Send some data (a command) to the mouse 
 * \param data the data to be sent
 */
void mouse_write(unsigned char data) {
  mouse_wait(READY_TO_SEND); //> Wait until the mouse is ready to receive a message
  outb(0x64, 0xd4); //> Tell the mouse we're going to send it something on port 0x60

  mouse_wait(READY_TO_SEND); //> Wait until the mouse is ready to receive a message
  outb(0x60, data); //> Send the data
}

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Read a message from the mouse
 * \returns the message read
 */
unsigned char mouse_read() {
  mouse_wait(READY_TO_READ); //> Wait until there's data for us to read (on port 0x60)
  return inb(0x60); //> Read from the mouse and return that value
}

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Initialize the mouse so that it sends interrupts over IRQ 12
 * \param type determines the byte we're waiting on and the value we're waiting for it to be
 */
void initialize_mouse() {

  // Enable the auxiliary mouse device
  mouse_wait(READY_TO_SEND);
  outb(0x64, 0xA8);
  
  // Enable interrupts on IRQ12
  mouse_wait(READY_TO_SEND);
  outb(0x64, 0x20); //> Send command "Get Compaq Status Byte"
  mouse_wait(READY_TO_READ);
  uint8_t status =(inb(0x60) | 2); //> Get the actual Compaq Status byte and turn on bit 1: Enable IRQ12
  mouse_wait(READY_TO_SEND);
  outb(0x64, 0x60); //> Send command "Set Compaq Status Byte"
  mouse_wait(READY_TO_SEND);
  outb(0x60, status); //> Send the modified Compaq Status byte
  
  // Reset mouse
  mouse_write(0xFF);
  mouse_read(); //> Read the ACK byte

  // Tell the mouse to use default settings 
  // (disable streaming, packet rate 100 per second, resolution 4 pixels per mm)
  mouse_write(0xF6);
  mouse_read(); //> Read the ACK byte
  
  // Enable packet streaming
  mouse_write(0xF4);
  mouse_read(); //> Read the ACK byte
}

// Functions below here were written by us

/**
 * Update the saved_pixels array, which stores what will be drawn in place of the
 * cursor once the cursor is moved, to be all one color. Thus, the cursor leaves
 * a trail of the specified color behind it.
 * \param color hexadecimal color code for the color to fill the array with
 */
void update_saved_pixels(int32_t color) {
  for(int i = 0; i < CURSOR_WIDTH * CURSOR_HEIGHT; i++) {
    saved_pixels[i] = color;
  }
}

/**
 * Stores the contents of the framebuffer which occur in a rectangle the size of the cursor at a specified
 * position. Contents are stored to the saved_pixel array, so that we can restore the background once the 
 * cursor moves.
 * \param x_start x-coordinate of the top left corner of the rectangle to save
 * \param y_start y-coordinate of the top left corner of the rectangle to save
 */
void save_background(int32_t x_start, int32_t y_start) {
  // Find the framebuffer and get some information about it
  unsigned char* framebuffer_start = (unsigned char*) global_framebuffer->framebuffer_addr; 
  uint8_t pixelwidth = global_framebuffer->framebuffer_bpp / 8;
  uint16_t pitch = global_framebuffer->framebuffer_pitch;

  // First index in the framebuffer where we should save pixels
  int index = (x_start * pixelwidth) + (y_start * pitch);

  uint32_t blue;
  uint32_t green;
  uint32_t red;
  uint32_t color;

  // Loop over the indices in the framebuffer where the cursor will be, saving as we go
  for(int y = 0; y < CURSOR_HEIGHT; y++) {
    for(int x = 0; x < CURSOR_WIDTH; x++) {
      // Get blue, green, and red components of the current pixel
      blue = framebuffer_start[index];
      green = (framebuffer_start[index + 1]) << 8;
      red = (framebuffer_start[index + 2]) << 16;
      
      // Turn RGB into hex color code and save the color
      color = blue | green | red;
      saved_pixels[x + y * CURSOR_WIDTH] = color;
      index += pixelwidth;
    }
    index += (pitch - (pixelwidth * CURSOR_WIDTH));
  }
}

/**
 * Move the cursor to a new location on the screen determined by the mouse's position, and
 * restore the background previously obscured by the cursor.
 */
void update_cursor() {

  // Store the current x and y position of the cursor
  int x_pos = data->x_pos;
  int y_pos = data->y_pos;

  uint8_t blue;
  uint8_t green;
  uint8_t red;

  // Loop over the saved_pixels array to restore the background where the cursor currently is
  for(int i = 0; i < CURSOR_WIDTH; i++) {
    for(int j = 0; j < CURSOR_HEIGHT; j++) {
      // Decompose hex into RGB
      blue = saved_pixels[i + j * CURSOR_WIDTH] & 0x0000FF;
      green = (saved_pixels[i + j * CURSOR_WIDTH] & 0x00FF00) >> 8;
      red = (saved_pixels[i + j * CURSOR_WIDTH] & 0xFF0000) >> 16;
      // Draw to the screen
      draw_pixel(i + x_pos, j + y_pos, red, green, blue);
    }
  }

  // Update the stored mouse position
  if(mousebytes->x_sb == 1) {
    data->x_pos += (mousebytes->x_move - 255) / SENSITIVITY;
  } else {
    data->x_pos += mousebytes->x_move / SENSITIVITY;
  }

  if(mousebytes->y_sb == 1) {
    data->y_pos -= (mousebytes->y_move - 255) / SENSITIVITY;
  } else {
    data->y_pos -= mousebytes->y_move / SENSITIVITY;
  }

  // If the mouse is off the screen, update its position to be just inside the screen 
  if(data->x_pos < 0) {
    data->x_pos = 0;
  }
  
  if(data->x_pos > global_framebuffer->framebuffer_width - (CURSOR_WIDTH + 1)) {
    data->x_pos = global_framebuffer->framebuffer_width - (CURSOR_WIDTH + 1);
  }

  if(data->y_pos < 0) {
    data->y_pos = 0;
  }
  
  if(data->y_pos > global_framebuffer->framebuffer_height - (CURSOR_HEIGHT + 1)) {
    data->y_pos = global_framebuffer->framebuffer_height - (CURSOR_HEIGHT + 1);
  }

  // Save the pixels that occur at the new mouse position (where we're about to move the cursor to)
  save_background(data->x_pos, data->y_pos);

  // Draw the cursor at the new position
  kdraw_rectangle(data->x_pos, data->y_pos, CURSOR_WIDTH, CURSOR_HEIGHT, WHITE);
}

/**
 * Make space for cursor-related data structures, initialize them to reasonable values, and draw the cursor.
 */
void initialize_cursor() {
  uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
  // Make space for processed mouse data (in the higher half because we unmap the lower half later)
  uintptr_t data_addr = 0xffff800034001000;
  vm_map(root, data_addr, 0, 1, 0);
  data = (mouse_data_t*) data_addr;

  // Make space for raw data from the mouse (in the higher half because we unmap the lower half later)
  uintptr_t mousebytes_addr = 0xffff800035001000;
  vm_map(root, mousebytes_addr, 0, 1, 0);
  mousebytes = (mouse_bytes_t*) mousebytes_addr;

  // Initialize the fields of our raw data struct
  data->x_pos = 100;
  data->y_pos = 100;
  data->left_click = false;
  data->right_click = false;
  data->middle_click = false;

  mouse_counter = 0; //> Keeps track of which byte in a 3-byte sequence we're receiving

  // Save the background where the cursor will be initially drawn 
  save_background(data->x_pos, data->y_pos);

  // Draw the cursor
  kdraw_rectangle(data->x_pos, data->y_pos, CURSOR_WIDTH, CURSOR_HEIGHT, WHITE);
}

/**
 * Given a byte from the mouse, store it in the appropriate place in the struct which holds raw mouse data.
 * \param packet a byte read from the mouse (one in a 3-byte sequence)
 */
void store_mouse_data(uint8_t packet) {
  switch(mouse_counter) {
    case 0 :
      // First byte in 3-byte sequence
      mousebytes->left = packet & 0x1; //> Is the mouse being left-clicked?
      data->left_click = mousebytes->left;
      // if(mousebytes->left == 1) {
      //   // gkprint_c('L', 200, 200, WHITE);
      //   data->left_click = true;
      // } else {
      //   // gkprint_c('N', 200, 200, WHITE);
      //   data->left_click = false;
      // }
      mousebytes->right = (packet & 0x2) >> 1; //> Is the mouse being right-clicked?
      data->right_click = mousebytes->right;
      // if(mousebytes->right == 1) {
      //   // gkprint_c('R', 200, 208, WHITE);
      //   data->right_click = true;
      // } else {
      //   // gkprint_c('S', 200, 208, WHITE);
      //   data->right_click = false;
      // }
      mousebytes->middle = (packet & 0x4) >> 2; //> Is the mouse being middle-clicked?
      data->middle_click = mousebytes->middle;
      // if(mousebytes->middle == 1) {
      //   // gkprint_c('M', 200, 216, WHITE);
      //   data->middle_click = true;
      // } else {
      //   // gkprint_c('O', 200, 216, WHITE);
      //   data->middle_click = false;
      // }

      // Testing unused bit - it should ALWAYS be 1. If the unused bit is not 1, skip bytes until we are back in alignment
      if(((packet & 0x8) >> 3) == 0) {
        //Error checking
        // gkprint_c('E', 500, 200, WHITE);
        // gkprint_c('R', 508, 200, WHITE);
        // gkprint_c('R', 516, 200, WHITE);

        return;
      }

      mousebytes->x_sb = (packet & 0x10) >> 4; //> Indicates sign (positive or negative) of direction of x-movement
      // gkprint_d(mousebytes->x_sb, 500, 500, WHITE);
      // for(int i = 600; i < 600 + CURSOR_WIDTH; i++) {
      //   for(int j = 600 + 50; j < 600 + 40 + CURSOR_HEIGHT; j++) {
      //     if(mousebytes->x_sb == 1) {
      //       draw_pixel(i, j, 0, 255, 0);
      //     } else {
      //       draw_pixel(i, j, 0, 0, 255);
      //     }
      //   }
      // }
      mousebytes->y_sb = (packet & 0x20) >> 5; //> Indicates sign (positive or negative) of direction of y-movement
      // gkprint_d(mousebytes->y_sb, 508, 500, WHITE);
      // for(int i = 650; i < 650 + CURSOR_WIDTH; i++) {
      //   for(int j = 600 + 50; j < 600 + 40 + CURSOR_HEIGHT; j++) {
      //     if(mousebytes->y_sb == 1) {
      //       draw_pixel(i+40, j+40, 255, 0, 0);
      //     } else {
      //       draw_pixel(i+40, j+40, 255, 0, 255);
      //     }
      //   }
      // }
      // mousebytes->x_overflow = packet & 0x40;

      // mousebytes->y_overflow = packet & 0x80;

      mouse_counter++;
      break;
    case 1 :
      // Second byte in 3-byte sequence
      mousebytes->x_move = packet;
      // gkprint_c(' ', 508, 600, WHITE);
      // gkprint_c(' ', 516, 600, WHITE);
      // gkprint_d(mousebytes->x_move, 500, 600, WHITE);
      mouse_counter++;
      break;
    case 2 :
      // Third byte in 3-byte sequence
      mousebytes->y_move = packet;
      // gkprint_c(' ', 508, 608, WHITE);
      // gkprint_c(' ', 516, 608, WHITE);
      // gkprint_d(mousebytes->y_move, 500, 608, WHITE);
      mouse_counter = 0;
      // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
      //   for(int j = test_y; j < test_y + CURSOR_HEIGHT; j++) {
      //     draw_pixel(i, j, 0, 0, 255);
      //   }
      // }
      // test_x += 5;
      // We have received a complete packet of data from the mouse! Process it
      update_cursor();
      break;
    default :
      break;
  }
}
