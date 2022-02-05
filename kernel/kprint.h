#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef void (*term_write_t)(const char *, size_t);
static term_write_t term_write = NULL;

void kprintf(const char *format, ...);

void set_term_write(term_write_t term_write);
