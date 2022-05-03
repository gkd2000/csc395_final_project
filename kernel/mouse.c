#include "mouse.h"

// Mouse initialization code from https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse

// Todo: reformat this, cite the author better, comment it clearly

//Note: This is a prototype version
void draw_cursor() {
  for(int i = data->x_pos; i < data->x_pos + CURSOR_WIDTH; i++) {
    for(int j = data->y_pos; j < data->y_pos + CURSOR_HEIGHT; j++) {
      draw_pixel(i, j, 255, 255, 255);
    }
  }
}

//Changes the processed_data location
void update_cursor() {
  data->x_pos += (mousebytes->x_sb == 1 ? mousebytes->x_move | 0xFFFFFF00 : mousebytes->x_move);
  data->y_pos += (mousebytes->y_sb == 1 ? mousebytes->y_move | 0xFFFFFF00 : mousebytes->y_move);

  draw_cursor();
}

void initialize_cursor() {
  data = malloc(sizeof(mouse_data_t)); //> struct which stores the processed/important data from the mouse
  mousebytes = malloc(sizeof(mouse_bytes_t)); //> struct which stores the raw data from the mouse
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

void MouseWait(unsigned char type) {
    int time_out = 100000;

    if (type == 0) {
        while (time_out--) {
            if ((inb(0x64) & 1) == 1)
                return;
        }
        return;
    }
    else {
        while (time_out--) {
            if ((inb(0x64) & 2) == 0)
                return;
        } 
        return;
    }
}

void MouseWrite(unsigned char data) {
    MouseWait(1);
    outb(0x64, 0xd4);

    MouseWait(1);
    outb(0x60, data);
}

unsigned char MouseRead() {
    MouseWait(0);
    return inb(0x60);
}

uint8_t function(uint8_t c) {
  return c + 2;
}

void InitialiseMouse() {

    // Enable the auxiliary mouse device
    MouseWait(1);
    outb(0x64, 0xA8);
    
    //Enable the interrupts
    MouseWait(1);
    outb(0x64, 0x20);
    MouseWait(0);
    uint8_t status =(inb(0x60) | 2);
    MouseWait(1);
    outb(0x64, 0x60);
    MouseWait(1);
    outb(0x60, status);
    
    MouseWrite(0xFF);
    MouseRead(); 
    //Tell the mouse to use default settings
    MouseWrite(0xF6);
    MouseRead();  //Acknowledge
    
    //Enable the mouse
    MouseWrite(0xF4);
    MouseRead();
}

void store_mouse_data(uint8_t packet) {
  switch(mouse_counter) {
    case 0 :
      mousebytes->left = packet & 0x1;
      mousebytes->right = packet & 0x2;
      mousebytes->middle = packet & 0x4;
      mousebytes->x_sb = packet & 0x10;
      mousebytes->y_sb = packet & 0x20;
      mousebytes->x_overflow = packet & 0x40;
      mousebytes->y_overflow = packet & 0x80;
      mouse_counter++;
      break;
    case 1 :
      mousebytes->x_move = packet;
      mouse_counter++;
      break;
    case 2 :
      mousebytes->y_move = packet;
      mouse_counter = 0;
      // Call a function which processes the packet
      update_cursor();
      break;
    default :
      break;
  }
}
