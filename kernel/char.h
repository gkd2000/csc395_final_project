#pragma once

#include <stdint.h>
#include <stddef.h>

#define SIZE 10


static char char_buffer[SIZE];
static int start = 0;
static volatile int count = 0;

static uint8_t lshift = 0;
static uint8_t rshift = 0;


uint8_t getchar(uint8_t ch);

void buffer_put(char c);


/**
 * Read one character from the keyboard buffer. If the keyboard buffer is empty this function will
 * block until a key is pressed.
 *
 * \returns the next character input from the keyboard
 */
char kgetc();
