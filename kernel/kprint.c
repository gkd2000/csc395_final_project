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

void gkprint_c(uint8_t c, uint32_t x_pos, uint32_t y_pos, uint32_t color) {
  uint8_t red = (color & 0xFF0000) >> 16;
  uint8_t green = (color & 0x00FF00) >> 8;
  uint8_t blue = color & 0x0000FF;
  int mask[8]={1,2,4,8,16,32,64,128};
  for(int w = 0; w < 8; w++) {
    // int char_h = 8;
    for(int h = 0; h < 8; h++) {
      draw_pixel(w+x_pos, h+y_pos, (letters[c][h] & mask[w]) * red, (letters[c][h] & mask[w]) * green, (letters[c][h] & mask[w]) * blue);
      // char_h++;
    }
  }
}

// Recursive helper function for kprint_d
// void gkprint_d_helper(uint64_t value, uint32_t x_pos, uint32_t y_pos, uint32_t color) {
//   if (value != 0) {
//     gkprint_c(value % 10 + 48, x_pos, y_pos, color); //> Print the least significant digit
    
//     gkprint_d_helper(value / 10, x_pos + 8, y_pos, color); //> Recursive call
//   }
// }

// /**
//  * Print a number in decimal to the terminal, without leading zeros
//  * \param value a nonnegative integer
//  */
// void gkprint_d(uint64_t value, uint32_t x_pos, uint32_t y_pos, uint32_t color) {
//   if (value != 0) {
//     gkprint_d_helper(value, x_pos, y_pos, color);
//   } else {
//     gkprint_c('0', x_pos, y_pos, color);
//   }
// }

void gkprint_d(uint64_t value, uint32_t x_pos, uint32_t y_pos, uint32_t color) {
  char arr[20];
  int counter = 0;
  for (size_t i = 19; i >= 0; i--) {
    int current_place = value % 10;
    arr[i] = current_place;
    value = value / 10;
    if (value == 0){
      counter = i;
      break;
    }
  }

  for(size_t i = counter; i < 20; i++) {
    if (arr[i] >= 0 && arr[i] <= 9) {
      gkprint_c(48 + arr[i], x_pos + (i - counter) * 8, y_pos, color);
    }
  }
}