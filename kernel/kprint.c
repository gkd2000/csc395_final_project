#include "kprint.h"

/**
 * Print a character to the terminal
 * \param c the character to be printed
 */
void kprint_c(char c) {
  term_write(&c, 1);
}

/**
 * Print a string to the terminal.
 * \param str the string to be printed. Must be null-terminated.
 */
void kprint_s(const char *str) {
  uint32_t length = strlen(str);
  term_write(str, length);
}

// Recursive helper function for kprint_d
void kprint_d_helper(uint64_t value) {
  if (value != 0)
  {
    kprint_d_helper(value / 10); //> Recursive call

    kprint_c(value % 10 + 48); //> Print the least significant digit
  }
}

/**
 * Print a number in decimal to the terminal, without leading zeros
 * \param value a nonnegative integer
 */
void kprint_d(uint64_t value) {
  if (value != 0) {
    kprint_d_helper(value);
  } else {
    kprint_c('0');
  }
}

// Recursive helper function for kprint_x
void kprint_x_helper(uint64_t value) {
  // Hexadecimal digits
  char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  if (value != 0) {
    kprint_x_helper(value / 16); //> Recursive call

    kprint_c(table[value % 16]); //> Print the least significant digit
  }
}

/**
 * Print a number in hexadecimal to the terminal, without leading zeros or a "0x" prefix
 * \param value a nonnegative integer
 */
void kprint_x(uint64_t value) {
  if (value != 0) {
    kprint_x_helper(value);
  } else {
    kprint_c('0');
  }
}

/**
 * Print a pointer to the terminal, preceded by "0x"
 * \param ptr the pointer to be printed
 */
void kprint_p(void *ptr) {
  term_write("0x", 2);
  kprint_x((uint64_t)ptr);
}

/**
 * Mimic the basic functionality of printf. Given a format string and
 * a variable number of additional arguments, print the format string, with the
 * additional arguments inserted, to the terminal.
 * \param format format string. Accepted format specifiers are: %c (character),
 * %s (string), %d (integer, printed in decimal), %x (integer, printed in hexadecimal),
 * and %p (pointer).
 */
void kprintf(const char *format, ...) {
  
  // Start processing variadic arguments
  va_list args;
  va_start(args, format);

  // Loop until we reach the end of the format string
  size_t index = 0;
  while (format[index] != '\0') {
    // Is the current charater a '%'?
    if (format[index] == '%') {
      // Yes, print the argument
      index++;
      switch (format[index]) {
      case '%':
        kprint_c('%');
        break;
      case 'c':
        kprint_c(va_arg(args, int));
        break;
      case 's':
        kprint_s(va_arg(args, char *));
        break;
      case 'd':
        kprint_d(va_arg(args, uint64_t));
        break;
      case 'x':
        kprint_x(va_arg(args, int64_t));
        break;
      case 'p':
        kprint_p(va_arg(args, void *));
        break;
      default:
        kprint_s("<not supported>");
      }
    } else {
      // No, just a normal character. Print it.
      kprint_c(format[index]);
    }
    index++;
  }

  // Finish handling variadic arguments
  va_end(args);
}

/**
 * Print a character to the screen in graphics mode, with foreground color as specified
 * and black background color
 * \param c     the character to be printed
 * \param x_pos x-coordinate (in pixels) where the top left corner of the character should go
 * \param y_pos y-coordinate (in pixels) where the top left corner of the character should go
 * \param color hexadecimal color code to print the character in
 */
void gkprint_c(uint8_t c, uint32_t x_pos, uint32_t y_pos, uint32_t color) {
  // Decompose color into RGB
  uint8_t red = (color & 0xFF0000) >> 16;
  uint8_t green = (color & 0x00FF00) >> 8;
  uint8_t blue = color & 0x0000FF;

  // Masks corresponding to each column of an 8x8 character
  int mask[8]={1,2,4,8,16,32,64,128};

  // Variable to store whether the current bit in the font bitmask is on or off
  int multiplier;

  // Iterate over the 8x8 square where the character will go, filling in pixels 
  for(int w = 0; w < 8; w++) {
    for(int h = 0; h < 8; h++) {
      // Find the entry in the bitmask (will be 0 or 1)
      multiplier = (letters[c][h] & mask[w]) >> w;
      // Draw the pixel to the screen in the given color if multiplier is 1, or black if multiplier is 0
      draw_pixel(w+x_pos, h+y_pos, multiplier * red, multiplier * green, multiplier * blue);
    }
  }
}

/**
 * Print an integer in decimal to the screen in graphics mode, without leading zeroes. 
 * Foreground color is specified by caller; background color is black
 * \param value the number to be printed
 * \param x_pos x-coordinate (in pixels) where the top left corner of the first digit should go
 * \param y_pos y-coordinate (in pixels) where the top left corner of the first digit should go
 * \param color hexadecimal color code to print the number in
 */
void gkprint_d(uint64_t value, uint32_t x_pos, uint32_t y_pos, uint32_t color) {
  char arr[20];
  size_t counter = 0;
  int current_place;

  // Fill in arr (starting from the last index) with the digits of value, from least to most significant
  for (counter = 19; counter >= 0; counter--) {
    // Set the current index in arr to be the rightmost digit of value
    current_place = value % 10;
    arr[counter] = current_place;

    value = value / 10;

    // We've recorded all the digits of value. Stop here and record where we are in the array
    if (value == 0){
      break;
    }
  }

  // Print the digits we recorded
  for(size_t i = counter; i < 20; i++) {
    if (arr[i] >= 0 && arr[i] <= 9) {
      gkprint_c(48 + arr[i], x_pos + (i - counter) * 8, y_pos, color);
    }
  }
}