#pragma once

#include <stdint.h>
#include <stddef.h>

// Size of the circular buffer for keyboard input
#define SIZE 10

static char char_buffer[SIZE]; //> The buffer itself
static int start = 0; //> The index from which to read characters
static volatile int count = 0; //> The number of elements in char_buffer

// Flags for left shift, right shift, and caps lock
static uint8_t lshift = 0; 
static uint8_t rshift = 0;
static uint8_t capslock = 0;

/**
 * Return the character corresponding to a keyboard event.
 * Side effect: if the code corresponding to backspace was received, remove an element
 * from the character buffer, so long as the character buffer is nonempty.
 * \param ch PS/2 scan code (from scan code set 1) corresponding to the keyboard event
 * \returns the character which was typed. If shift or capslock was engaged 
 *          and the key pressed was one that is affected by this, then return the
 *          appropriate character. If an unprintable character was pressed, return 0.
 */
uint8_t getchar(uint8_t ch);

/**
 * Remove the most recently inserted item from the circular buffer
 * (note that ordinarily we remove things from the buffer following FIFO)
 */
void buffer_delete();

/**
 * Insert a character into the circular buffer char_buffer
 * \param c the character to be inserted
 */
void buffer_put(char c);

/**
 * Read one character from the keyboard buffer. If the keyboard buffer is empty this function will
 * block until a key is pressed.
 *
 * \returns the next character input from the keyboard
 */
char kgetc();
