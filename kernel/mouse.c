#include "mouse.h"

int test_x = 50;
int test_y = 50;
int mouse_counter;

uint32_t saved_pixels[CURSOR_HEIGHT * CURSOR_WIDTH]; // Store the colors in hex

// Mouse initialization code from https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Wait for a response from the mouse. 
 * \param type determines the byte we're waiting on and the value we're waiting for it to be
 */
void mouse_wait(unsigned char type) {
  // How long do we wait before we give up?
  int time_out = 100000;

  if (type == 0) {
    // Wait for the least significant bit returned by the mouse to be set
    while (time_out--) {
      if ((inb(0x64) & 1) == 1)
        return;
    }
    return;
  } else {
    // Wait for the second-least significant bit returned by the mouse to be turned off
    while (time_out--) {
      if ((inb(0x64) & 2) == 0)
        return;
    } 
    return;
  }
}

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Send some data (a message) to the mouse 
 * \param data the data to be sent
 */
void mouse_write(unsigned char data) {
  // Tell the mouse we're going to send it something
  mouse_wait(1);
  outb(0x64, 0xd4);

  // Wait for the mouse to be ready and then send data
  mouse_wait(1);
  outb(0x60, data);
}

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Read a message from the mouse
 * \returns the message read
 */
unsigned char mouse_read() {
  mouse_wait(0);
  // Read from the mouse and return that value
  return inb(0x60);
}

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Initialize the mouse so that it sends interrupts over IRQ 12
 * \param type determines the byte we're waiting on and the value we're waiting for it to be
 */
void initialize_mouse() {

  // Enable the auxiliary mouse device
  mouse_wait(1);
  outb(0x64, 0xA8);
  
  //Enable the interrupts
  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  uint8_t status =(inb(0x60) | 2);
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, status);
  
  mouse_write(0xFF);
  mouse_read(); 
  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read(); 
  
  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();
}

// Stuff below here is stuff we wrote

//Note: This is a prototype version
void draw_cursor() {
    // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 20; j < test_y + 20 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 255, 0, 255);
    //     }
    //   }
    //   test_x += 5;
  for(int i = data->x_pos; i < data->x_pos + CURSOR_WIDTH; i++) {
    for(int j = data->y_pos; j < data->y_pos + CURSOR_HEIGHT; j++) {
      draw_pixel(i, j, 255, 255, 255);
    }
  }



  // gkprint_c('A', 500, 500, WHITE);
  // gkprint_d(6, 508, 500, WHITE);
  // gkprint_d(17, 516, 500, WHITE);
  // gkprint_d(123456, 532, 500, WHITE);
}

void save_background(int32_t x_start, int32_t y_start) {
  unsigned char* framebuffer_start = (unsigned char*) global_framebuffer->framebuffer_addr;
  for(int i = 0; i < CURSOR_WIDTH; i++) {
    for(int j = 0; j < CURSOR_HEIGHT; j++) {
      int index = (x_start + i) * (global_framebuffer->framebuffer_bpp / 8) + ((y_start + j) * global_framebuffer->framebuffer_pitch);
      uint32_t blue = framebuffer_start[index];
      uint32_t green = (framebuffer_start[index + 1]) << 8;
      uint32_t red = (framebuffer_start[index + 2]) << 16;
      uint32_t color = blue | green | red;
      saved_pixels[i + j * CURSOR_WIDTH] = color;
    }
  }
}

//Changes the processed_data location
void update_cursor() {
  // data->x_pos += (mousebytes->x_sb == 1 ? mousebytes->x_move | 0xFFFFFF00 : mousebytes->x_move) / 50;
  // data->y_pos += (mousebytes->y_sb == 1 ? mousebytes->y_move | 0xFFFFFF00 : mousebytes->y_move) / 50;
  // data->x_pos += (mousebytes->x_sb == 1 ? (~mousebytes->x_move + 1) * -1 : mousebytes->x_move) / 50;
  // data->y_pos += (mousebytes->y_sb == 1 ? (~mousebytes->y_move + 1) * -1 : mousebytes->y_move) / 50;
  gkprint_c('r', 750, 185, WHITE);
  gkprint_c('g', 800, 185, WHITE);
  gkprint_c('b', 850, 185, WHITE);
  // int i_print;
  // int j_print;
  // for(int i = data->x_pos; i < data->x_pos + CURSOR_WIDTH; i++) {
  //   for(int j = data->y_pos; j < data->y_pos + CURSOR_HEIGHT; j++) {
  //     i_print = i - data->x_pos;
  //     j_print = j - data->y_pos;
  //     gkprint_c('T', 700, 100, WHITE);
  //     uint8_t blue = saved_pixels[i_print + j_print * CURSOR_WIDTH] & 0x0000FF;
  //     uint8_t green = (saved_pixels[i_print + j_print * CURSOR_WIDTH] & 0x00FF00) >> 8;
  //     uint8_t red = (saved_pixels[i_print + j_print * CURSOR_WIDTH] & 0xFF0000) >> 16;
  //     gkprint_c('T', 708, 100, WHITE);
  //     gkprint_d(i + j*CURSOR_WIDTH, 700, 200 + ((i + j*CURSOR_WIDTH) * 8), WHITE);
  //     gkprint_d(red, 750, 200 + ((i_print + j_print*CURSOR_WIDTH) * 8), WHITE);
  //     gkprint_d(green, 800, 200 + ((i_print + j_print*CURSOR_WIDTH) * 8), WHITE);
  //     gkprint_d(blue, 850, 200 + ((i_print + j_print*CURSOR_WIDTH) * 8), WHITE);
  //     draw_pixel(i, j, (saved_pixels[i_print + j_print * CURSOR_WIDTH] & 0xFF0000) >> 16, (saved_pixels[i_print + j_print * CURSOR_WIDTH] & 0x00FF00) >> 8, saved_pixels[i_print + j_print * CURSOR_WIDTH] & 0x0000FF);
  //   }
  // }

  int x_pos = data->x_pos;
  int y_pos = data->y_pos;
  for(int i = 0; i < CURSOR_WIDTH; i++) {
    for(int j = 0; j < CURSOR_HEIGHT; j++) {
      gkprint_c('T', 700, 100, WHITE);
      uint8_t blue = saved_pixels[i + j * CURSOR_WIDTH] & 0x0000FF;
      uint8_t green = (saved_pixels[i + j * CURSOR_WIDTH] & 0x00FF00) >> 8;
      uint8_t red = (saved_pixels[i + j * CURSOR_WIDTH] & 0xFF0000) >> 16;
      gkprint_c('T', 708, 100, WHITE);
      gkprint_d(i + j*CURSOR_WIDTH, 700, 200 + ((i + j*CURSOR_WIDTH) * 8), WHITE);
      gkprint_d(red, 750, 200 + ((i + j * CURSOR_WIDTH) * 8), WHITE);
      gkprint_d(green, 800, 200 + ((i + j * CURSOR_WIDTH) * 8), WHITE);
      gkprint_d(blue, 850, 200 + ((i + j * CURSOR_WIDTH) * 8), WHITE);
      draw_pixel(i + x_pos, j + y_pos, (saved_pixels[i + j * CURSOR_WIDTH] & 0xFF0000) >> 16, (saved_pixels[i + j * CURSOR_WIDTH] & 0x00FF00) >> 8, saved_pixels[i + j * CURSOR_WIDTH] & 0x0000FF);
    }
  }

  if(mousebytes->x_sb == 1) {
    // data->x_pos -= ((~(mousebytes->x_move)) + 1) / 50;
    data->x_pos += (mousebytes->x_move - 255) / 20;
  } else {
    data->x_pos += mousebytes->x_move / 20;
  }

  if(mousebytes->y_sb == 1) {
    // data->y_pos -= ((~(mousebytes->y_move)) + 1) / 50;
    data->y_pos -= (mousebytes->y_move - 255) / 20;
  } else {
    data->y_pos -= mousebytes->y_move / 20;
  }

  if(data->x_pos < 0) {
    data->x_pos = 0;
  }
  
  if(data->x_pos > global_framebuffer->framebuffer_width - 6) {
    data->x_pos = global_framebuffer->framebuffer_width - 6;
  }

  if(data->y_pos < 0) {
    data->y_pos = 0;
  }
  
  if(data->y_pos > global_framebuffer->framebuffer_height - 6) {
    data->y_pos = global_framebuffer->framebuffer_height - 6;
  }

  gkprint_d(data->x_pos, 500, 300, WHITE);
  gkprint_d(data->y_pos, 500, 308, WHITE);

    // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 10; j < test_y + 10 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 255, 0, 255);
    //     }
    //   }
    //   test_x += 5;

  save_background(data->x_pos, data->y_pos);

  draw_cursor();
}

void initialize_cursor() {
  data = malloc(sizeof(mouse_data_t)); //> struct which stores the processed/important data from the mouse
  mousebytes = malloc(sizeof(mouse_bytes_t)); //> struct which stores the raw data from the mouse

//   if(mousebytes == NULL) {
//       for(int i = 100; i < 120; i++) {
//         for(int j = 100 + 10; j < 120; j++) {
//           draw_pixel(i, j, 255, 0, 255);
//         }
//       }
//   }

//   uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
//   uintptr_t addr_mousebytes = 0x500000;
//   if(vm_map(root, addr_mousebytes, true, true, false)) {
//       for(int i = 100; i < 120; i++) {
//         for(int j = 100 + 10; j < 120; j++) {
//           draw_pixel(i, j, 255, 0, 255);
//         }
//       }
//   } else {
//       for(int i = 100; i < 120; i++) {
//         for(int j = 100 + 10; j < 120; j++) {
//           draw_pixel(i, j, 255, 0, 0);
//         }
//       }
//   }
  data->x_pos = 100;
  data->y_pos = 100;
  data->click = false;
  mouse_counter = 0;
  save_background(data->x_pos, data->y_pos);

  //Draw the cursor from here!
  draw_cursor();
}

//Handler calls this function when the mouse moves (?)
// void mouse_input() {

// }

void store_mouse_data(uint8_t packet) {
  switch(mouse_counter) {
    case 0 :
    // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y; j < test_y + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 255, 0, 0);
    //     }
    //   }
    //   test_x += 5;
    //   if(packet == NULL || packet == 0) {
    //     for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //       for(int j = test_y + 50; j < test_y + 50 + CURSOR_HEIGHT; j++) {
    //         draw_pixel(i, j, 255, 0, 0);
    //       }
    //     }
    //   } else {
    //     for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //       for(int j = test_y + 50; j < test_y + 50 + CURSOR_HEIGHT; j++) {
    //         draw_pixel(i, j, 0, 255, 0);
    //       }
    //     }
    //   }
      mousebytes->left = packet & 0x1;
      if(mousebytes->left == 1) {
        gkprint_c('L', 200, 200, WHITE);
      } else {
        gkprint_c('N', 200, 200, WHITE);
      }
    //   for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 50; j < test_y + 40 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 0, 255, 0);
    //     }
    //   }
    //   test_x += 5;
      mousebytes->right = (packet & 0x2) >> 1;
      if(mousebytes->right == 1) {
        gkprint_c('R', 200, 208, WHITE);
      } else {
        gkprint_c('S', 200, 208, WHITE);
      }
    //   for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 50; j < test_y + 40 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 0, 255, 0);
    //     }
    //   }
    //   test_x += 5;
      mousebytes->middle = (packet & 0x4) >> 2;
      if(mousebytes->middle == 1) {
        gkprint_c('M', 200, 216, WHITE);
      } else {
        gkprint_c('O', 200, 216, WHITE);
      }
    //   for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 50; j < test_y + 40 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 0, 255, 0);
    //     }
    //   }
      // test_x += 5;

      //Testing unused bit - it should ALWAYS be 1

      if(((packet & 0x8) >> 3) == 0) {
        gkprint_c('E', 500, 200, WHITE);
        gkprint_c('R', 508, 200, WHITE);
        gkprint_c('R', 516, 200, WHITE);
      }

      mousebytes->x_sb = (packet & 0x10) >> 4;
      gkprint_d(mousebytes->x_sb, 500, 500, WHITE);
      for(int i = 600; i < 600 + CURSOR_WIDTH; i++) {
        for(int j = 600 + 50; j < 600 + 40 + CURSOR_HEIGHT; j++) {
          if(mousebytes->x_sb == 1) {
            draw_pixel(i, j, 0, 255, 0);
          } else {
            draw_pixel(i, j, 0, 0, 255);
          }
        }
      }
      // test_x += 5;
      mousebytes->y_sb = (packet & 0x20) >> 5;
      gkprint_d(mousebytes->y_sb, 508, 500, WHITE);
      for(int i = 650; i < 650 + CURSOR_WIDTH; i++) {
        for(int j = 600 + 50; j < 600 + 40 + CURSOR_HEIGHT; j++) {
          if(mousebytes->y_sb == 1) {
            draw_pixel(i+40, j+40, 255, 0, 0);
          } else {
            draw_pixel(i+40, j+40, 255, 0, 255);
          }
        }
      }
      // test_x += 5;
      mousebytes->x_overflow = packet & 0x40;
      // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
      //   for(int j = test_y + 50; j < test_y + 40 + CURSOR_HEIGHT; j++) {
      //     draw_pixel(i, j, 0, 255, 0);
      //   }
      // }
      // test_x += 5;
      mousebytes->y_overflow = packet & 0x80;
      // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
      //   for(int j = test_y + 50; j < test_y + 40 + CURSOR_HEIGHT; j++) {
      //     draw_pixel(i, j, 0, 255, 0);
      //   }
      // }
      // test_x += 5;
      mouse_counter++;
      // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
      //   for(int j = test_y; j < test_y + CURSOR_HEIGHT; j++) {
      //     draw_pixel(i, j, 255, 255, 0);
      //   }
      // }
      // test_x += 5;
      break;
    case 1 :
      // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
      //   for(int j = test_y; j < test_y + CURSOR_HEIGHT; j++) {
      //     draw_pixel(i, j, 0, 255, 0);
      //   }
      // }
      // test_x += 5;
      mousebytes->x_move = packet;
      gkprint_c(' ', 508, 600, WHITE);
      gkprint_c(' ', 516, 600, WHITE);
      gkprint_d(mousebytes->x_move, 500, 600, WHITE);
      mouse_counter++;
      break;
    case 2 :
      mousebytes->y_move = packet;
      gkprint_c(' ', 508, 608, WHITE);
      gkprint_c(' ', 516, 608, WHITE);
      gkprint_d(mousebytes->y_move, 500, 608, WHITE);
      mouse_counter = 0;
      // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
      //   for(int j = test_y; j < test_y + CURSOR_HEIGHT; j++) {
      //     draw_pixel(i, j, 0, 0, 255);
      //   }
      // }
      // test_x += 5;
      // Call a function which processes the packet
      update_cursor();
      break;
    default :
      break;
  }
}

void do_nothing(uint8_t packet) {
  return;
}
