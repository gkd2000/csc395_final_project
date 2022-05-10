#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "unistd.h"
#include "port.h"
#include "pic.h"
#include "graphics.h"
#include "stivale-hdr.h"
#include "mem.h"
#include "font.h"

#define CURSOR_WIDTH 5
#define CURSOR_HEIGHT 5

// Constants for sending and reading messages to/from the mouse
#define READY_TO_READ 0
#define READY_TO_SEND 1

#define SENSITIVITY 5

typedef struct mouse_data {
    int32_t x_pos;
    int32_t y_pos;
    bool click;
} mouse_data_t;

//This is 3 bytes large
typedef struct mouse_bytes {
  uint8_t left : 1;
  uint8_t right : 1;
  uint8_t middle : 1;
  uint8_t unused : 1;
  uint8_t x_sb : 1;
  uint8_t y_sb : 1;
  uint8_t x_overflow : 1; //Not useful
  uint8_t y_overflow : 1; //Not useful
  uint8_t x_move;
  uint8_t y_move;
} __attribute__((packed)) mouse_bytes_t;

mouse_data_t* data;
mouse_bytes_t* mousebytes;

void initialize_mouse();

void store_mouse_data(uint8_t packet);

void do_nothing(uint8_t packet);

void initialize_cursor();