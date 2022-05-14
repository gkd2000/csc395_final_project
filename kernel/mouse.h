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
#include "assembly.h"

#define CURSOR_WIDTH 5
#define CURSOR_HEIGHT 5

// Constants for sending and reading messages to/from the mouse
#define READY_TO_READ 0
#define READY_TO_SEND 1

#define SENSITIVITY 5

// Struct for processed mouse data
typedef struct mouse_data {
    int32_t x_pos;
    int32_t y_pos;
    bool left_click;
    bool right_click;
    bool middle_click;
} mouse_data_t;

// Struct for raw mouse data. Matches the format of the 3-byte
// packets received from mouse
typedef struct mouse_bytes {
  uint8_t left : 1;
  uint8_t right : 1;
  uint8_t middle : 1;
  uint8_t unused : 1;
  uint8_t x_sb : 1;
  uint8_t y_sb : 1;
  uint8_t x_overflow : 1; // Not useful
  uint8_t y_overflow : 1; // Not useful
  uint8_t x_move;
  uint8_t y_move;
} __attribute__((packed)) mouse_bytes_t;

mouse_data_t* data;
mouse_bytes_t* mousebytes;
bool restore_background; //> Probably take this out, unused right now, but part of an alternative solution to overwriting saved_pixels

/**
 * Source: https://forum.osdev.org/viewtopic.php?f=1&t=49913&hilit=mouse, user elrond06
 * Initialize the mouse so that it sends interrupts over IRQ 12
 * \param type determines the byte we're waiting on and the value we're waiting for it to be
 */
void initialize_mouse();

/**
 * Given a byte from the mouse, store it in the appropriate place in the struct which holds raw mouse data.
 * \param packet a byte read from the mouse (one in a 3-byte sequence)
 */
void store_mouse_data(uint8_t packet);

/**
 * Make space for cursor-related data structures, initialize them to reasonable values, and draw the cursor.
 */
void initialize_cursor();

/**
 * Update the saved_pixels array, which stores what will be drawn in place of the
 * cursor once the cursor is moved, to be all one color. Thus, the cursor leaves
 * a trail of the specified color behind it.
 * \param color hexadecimal color code for the color to fill the array with
 */
void update_saved_pixels(int32_t color);
