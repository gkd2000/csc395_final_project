#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <syscall.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define WHITE 0xFFFFFF

/**
 * Draw a pixel at the specified location on the screen
 * \param x_pos x-coordinate (in pixels) of the pixel
 * \param y_pos y-coordinate (in pixels) of the pixel
 * \param r     red component of the color
 * \param g     green component of the color
 * \param b     blue component of the color
 * \returns 1
 */
int64_t drawpixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b);

/**
 * Get the most recent data from the mouse
 * \param mouse_data struct (of type mouse_data_t) to populate with recent data
 * \returns 1
 */
int64_t readmouse(uintptr_t mouse_data);

/**
 * Update the array which stores what will be drawn in place of the cursor once the cursor is moved. 
 * Set all values of this array to be one color. Thus, the cursor leaves a trail of the specified 
 * color behind it.
 * \param color hexadecimal color code for the color to fill the array with
 * \returns 1
 */
int64_t update_cursor_background(int32_t color);

/**
 * In graphics mode, write a specified number of characters from buffer to the screen at the requested position.
 * \param x_pos  x-coordinate (in pixels) of the top left corner of the beginning of the string
 * \param y_pos  y-coordinate (in pixels) of the top left corner of the beginning of the string
 * \param color  hexadecimal color code specifying the color of the text
 * \param buffer array of characters to print
 * \param size   number of characters to print. Must be less than the number of characters in buffer
 * \returns 1
 */
int64_t gwrite(uint32_t x_pos, uint32_t y_pos, uint32_t color, void* buffer, size_t size);

/**
 * In graphics mode, make the screen all black and then load and execute a program specified by file.
 * \param file the program to execute. Must be a null-terminated string
 * \param argv unused
 * \returns -1 if no module matching file was found.
 *    Otherwise, an executable will be run, and so this function should not return.
 */
int64_t gexec(char* file, char* argv[]);