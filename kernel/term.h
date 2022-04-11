#pragma once

#include <stdint.h>
#include <stddef.h>

#include "mem.h"
#include "port.h"

void term_init();

void term_putchar(char c);

void term_write(const char *arr, size_t s);
