#include "mouse.h"
#include "font.h"

int test_x = 50;
int test_y = 50;
int mouse_counter;

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



	int mask[8]={1,2,4,8,16,32,64,128};
  for(int w = 0; w < 8; w++) {
    // int char_h = 8;
    for(int h = 0; h < 8; h++) {
      draw_pixel(w+500, h+500, (letters[65][h] & mask[w]) * 255, (letters[65][h] & mask[w]) * 255, (letters[65][h] & mask[w]) * 255);
      // char_h++;
    }
  }
}

//Changes the processed_data location
void update_cursor() {
  // data->x_pos += (mousebytes->x_sb == 1 ? mousebytes->x_move | 0xFFFFFF00 : mousebytes->x_move);
  // data->y_pos += (mousebytes->y_sb == 1 ? mousebytes->y_move | 0xFFFFFF00 : mousebytes->y_move);
  data->x_pos += (mousebytes->x_sb == 1 ? (~mousebytes->x_move + 1) * -1 : mousebytes->x_move) / 50;
  data->y_pos += (mousebytes->y_sb == 1 ? (~mousebytes->y_move + 1) * -1 : mousebytes->y_move) / 5;

    // for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 10; j < test_y + 10 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 255, 0, 255);
    //     }
    //   }
    //   test_x += 5;
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
  data->x_pos = 0;
  data->y_pos = 0;
  data->click = false;
  mouse_counter = 0;

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
    //   for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 50; j < test_y + 40 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 0, 255, 0);
    //     }
    //   }
    //   test_x += 5;
      mousebytes->right = packet & 0x2;
    //   for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 50; j < test_y + 40 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 0, 255, 0);
    //     }
    //   }
    //   test_x += 5;
      mousebytes->middle = packet & 0x4;
    //   for(int i = test_x; i < test_x + CURSOR_WIDTH; i++) {
    //     for(int j = test_y + 50; j < test_y + 40 + CURSOR_HEIGHT; j++) {
    //       draw_pixel(i, j, 0, 255, 0);
    //     }
    //   }
      // test_x += 5;
      mousebytes->x_sb = packet & 0x10;
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
      mousebytes->y_sb = packet & 0x20;
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
      mouse_counter++;
      break;
    case 2 :
      mousebytes->y_move = packet;
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
