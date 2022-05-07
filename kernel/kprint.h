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

void gkprint_c(uint8_t c, uint32_t x_pos, uint32_t y_pos, uint32_t color);

void gkprint_d(uint64_t value, uint32_t x_pos, uint32_t y_pos, uint32_t color);
