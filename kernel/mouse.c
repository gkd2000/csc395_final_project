#include "mouse.h"

// Mouse initialization code from https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse

// Todo: reformat this, cite the author better, comment it clearly

void initialize_cursor() {
  mouse_data_t* data = malloc(sizeof(mouse_data_t));
  data->x_pos = 0;
  data->y_pos = 0;
  data->click = false;
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

void InitialiseMouse() {
    unsigned char _status;  //unsigned char

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
