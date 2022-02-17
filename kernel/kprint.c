#include "kprint.h"

/**
 * Set the term_write function, which allows us to print output to the terminal
 * \param t function pointer to the function we want to set term_write to be
 */
void set_term_write(term_write_t t)
{
  term_write = t;
}

/**
 * Counts the number of characters, excluding the null terminator, in a string
 * \param str the string to count characters of
 */
uint32_t strlen(const char *str)
{
  uint32_t length = 0;
  const char *temp = str;

  // Iterate through str, incrementing our counter
  while (*temp != '\0')
  {
    length++;
    temp++;
  }

  return length;
}

// Print a character to the terminal
void kprint_c(char c)
{
  term_write(&c, 1);
}

// Print a string to the terminal.
// The input string, str, must be null-terminated.
void kprint_s(const char *str)
{
  uint32_t length = strlen(str);
  term_write(str, length);
}

void kprint_d_helper(uint64_t value) {
  if (value != 0)
  {
    kprint_d_helper(value / 10); //> Recursive call

    kprint_c(value % 10 + 48); //> Print the least significant digit
  }
}

// Print a number in decimal to the terminal, without leading zeros
void kprint_d(uint64_t value)
{
  if (value != 0)
  {
    kprint_d_helper(value);
  } else {
    kprint_c('0');
  }
}

void kprint_x_helper(uint64_t value)
{
  // Hexadecimal digits
  char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  if (value != 0)
  {
    kprint_x_helper(value / 16); //> Recursive call

    kprint_c(table[value % 16]); //> Print the least significant digit
  };
}


// Print a number in hexadecimal to the terminal, without leading zeros
void kprint_x(uint64_t value) {
  if (value != 0)
  {
    kprint_x_helper(value);
  }
  else
  {
    kprint_c('0');
  }
}

// Print a pointer to the terminal, preceded by "0x"
void kprint_p(void *ptr)
{
  term_write("0x", 2);
  kprint_x((uint64_t)ptr);
}

/**
 * Mimic the basic functionality of printf. Given a format string and
 * a variable number of additional arguments, print the format string, with the
 * additional arguments inserted, to the terminal.
 * Accepted format specifiers are: %c (character), %s (string),
 * %d (integer, printed in decimal), %x (integer, printed in hexadecimal),
 * and %p (pointer).
 */
void kprintf(const char *format, ...)
{
  // Start processing variadic arguments
  va_list args;
  va_start(args, format);

  // Loop until we reach the end of the format string
  size_t index = 0;
  while (format[index] != '\0')
  {
    // Is the current charater a '%'?
    if (format[index] == '%')
    {
      // Yes, print the argument
      index++;
      switch (format[index])
      {
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
    }
    else
    {
      // No, just a normal character. Print it.
      kprint_c(format[index]);
    }
    index++;
  }

  // Finish handling variadic arguments
  va_end(args);
}


