#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#include "term.h"
#include "graphics.h"
#include "font.h"

/**
 * Mimic the basic functionality of printf. Given a format string and
 * a variable number of additional arguments, print the format string, with the
 * additional arguments inserted, to the terminal.
 * \param format format string. Accepted format specifiers are: %c (character),
 * %s (string), %d (integer, printed in decimal), %x (integer, printed in hexadecimal),
 * and %p (pointer).
 */
void kprintf(const char *format, ...);

/**
 * Print a character to the screen in graphics mode, with foreground color as specified
 * and black background color
 * \param c     the character to be printed
 * \param x_pos x-coordinate (in pixels) where the top left corner of the character should go
 * \param y_pos y-coordinate (in pixels) where the top left corner of the character should go
 * \param color hexadecimal color code to print the character in
 */
void gkprint_c(uint8_t c, uint32_t x_pos, uint32_t y_pos, uint32_t color);

/**
 * Print an integer in decimal to the screen in graphics mode, without leading zeroes. 
 * Foreground color is specified by caller; background color is black
 * \param value the number to be printed
 * \param x_pos x-coordinate (in pixels) where the top left corner of the first digit should go
 * \param y_pos y-coordinate (in pixels) where the top left corner of the first digit should go
 * \param color hexadecimal color code to print the number in
 */
void gkprint_d(uint64_t value, uint32_t x_pos, uint32_t y_pos, uint32_t color);
