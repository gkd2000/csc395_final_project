#include "kprint.h"

void set_term_write(term_write_t t) {
  term_write = t;
}


uint32_t strlen(const char *str)
{
  uint32_t length = 0;
  const char *temp = str;
  while (*temp != '\0')
  {
    length++;
    temp++;
  }

  return length;
}


void kprint_c(char c)
{
  term_write(&c, 1);
}


void kprint_s(const char *str)
{
  uint32_t length = strlen(str);
  term_write(str, length);
}

void kprint_d(uint64_t value)
{
  if (value != 0)
  {
    kprint_d(value / 10);

    kprint_c(value % 10 + 48);
  }
}

void kprint_x(uint64_t value)
{
  char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  if (value != 0)
  {
    kprint_x(value / 16);

    kprint_c(table[value % 16]);
  };
}

void kprint_p(void *ptr)
{
  term_write("0x", 2);
  kprint_x((uint64_t)ptr);
}



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


