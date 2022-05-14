#pragma once

#include <stdint.h>

#include "stivale-hdr.h"
#include "kprint.h"
#include "mouse.h"

#define WHITE 0xFFFFFF //> Hexadecimal color white

/**
 * Set the color of a single pixel
 * Note that for specifying position, the coordinate system starts at (0, 0) in the top left corner of the screen.
 * The x-component increases as you move rightwards across the screen.
 * The y-component increases as you move down the screen.
 * \param x_pos x-coordinate (in pixels) of the pixel
 * \param y_pos y-coordinate (in pixels) of the pixel
 * \param r     red component of the color
 * \param g     green component of the color
 * \param b     blue component of the color
 */
void draw_pixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b);

/**
 * Based on code from https://wiki.osdev.org/Drawing_In_a_Linear_Framebuffer
 * Draw a rectangle with specified position, dimension, and color
 * Note that for specifying position, the coordinate system starts at (0, 0) in the top left corner of the screen.
 * The x-component increases as you move rightwards across the screen.
 * The y-component increases as you move down the screen.
 * \param x_pos  x-coordinate (in pixels) of the top left corner of the rectangle
 * \param y_pos  y-coordinate (in pixels) of the top left corner of the rectangle
 * \param width  width of the rectangle in pixels
 * \param height height of the rectangle in pixels 
 * \param color  color of the rectangle as a hexadecimal color code
 */ 
void draw_rectangle(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height, uint32_t color);

/**
 * Sets all pixels on the screen to black except the cursor
 */ 
void clear_screen();