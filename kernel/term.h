#pragma once

#include <stdint.h>
#include <stddef.h>

#include "mem.h"
#include "port.h"

// Initialize the terminal
void term_init();

// Write one character to the terminal
void term_putchar(char c);

// Write s characters from arr to the terminal
void term_write(const char *arr, size_t s);
