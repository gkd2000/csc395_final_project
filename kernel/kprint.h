#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

// Typedef a function pointer which corresponds to the inputs and outputs of the term_write function
typedef void (*term_write_t)(const char *, size_t);
// Initialize term_write to NULL
static term_write_t term_write = NULL;

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
 * Set the term_write function, which allows us to print output to the terminal
 * \param t function pointer to the function we want to set term_write to be
 */
void set_term_write(term_write_t term_write);
