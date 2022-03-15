#include "unistd.h"

size_t read(int fildes, void *buf, size_t nbyte) {
  return syscall(SYS_read, fildes, (intptr_t)buf, nbyte);
}

// size_t write(int fildes, void *buf, size_t nbyte) {
//   return syscall(SYS_write, fildes, buf, nbyte);
// }

// // Recursive helper function for kprint_d
// void print_d_helper(uint64_t value)
// {
//   if (value != 0)
//   {
//     print_d_helper(value / 10); //> Recursive call

//     uint64_t temp = value % 10 + 48;
//     write(STDOUT, &temp, 1); //> Print the least significant digit
//   }
// }

// // Recursive helper function for kprint_x
// void print_x_helper(uint64_t value)
// {
//   // Hexadecimal digits
//   char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
//   if (value != 0)
//   {
//     print_x_helper(value / 16); //> Recursive call

//     write(STDOUT, &table[value % 16], 1); //> Print the least significant digit
//   }
// }

// /**
//  * Mimic the basic functionality of printf. Given a format string and
//  * a variable number of additional arguments, print the format string, with the
//  * additional arguments inserted, to the terminal.
//  * \param format format string. Accepted format specifiers are: %c (character),
//  * %s (string), %d (integer, printed in decimal), %x (integer, printed in hexadecimal),
//  * and %p (pointer).
//  */
// void printf(const char *format, ...)
// {
//   // Start processing variadic arguments
//   va_list args;
//   va_start(args, format);

//   // Loop until we reach the end of the format string
//   size_t index = 0;
//   while (format[index] != '\0')
//   {
//     // Is the current charater a '%'?
//     if (format[index] == '%')
//     {
//       // Yes, print the argument
//       index++;
//       switch (format[index])
//       {
//       case '%':
//         write(STDOUT, &format[index], 1);
//         break;
//       case 'c':
//         char temp = va_arg(args, int);
//         write(STDOUT, &temp, 1);
//         break;
//       case 's':
//         char *temp = va_arg(args, char *);
//         write(STDOUT, temp, strlen(temp));
//         break;
//       case 'd':
//         uint64_t value = va_arg(args, uint64_t);
//         if (value != 0) {
//           print_d_helper(value);
//         }
//         else {
//           char temp = '0';
//           write(STDOUT, &temp, 1);
//         }
//         break;
//       case 'x':
//         uint64_t value = va_arg(args, uint64_t);
//         if (value != 0) {
//           print_x_helper(value);
//         }
//         else {
//           char temp = '0';
//           write(STDOUT, &temp, 1);
//         }
//         break;
//       case 'p':
//         write(STDOUT, "0x", 2);
//         uint64_t value = va_arg(args, uint64_t);
//         if (value != 0) {
//           print_x_helper(value);
//         }
//         else {
//           char temp = '0';
//           write(STDOUT, &temp, 1);
//         }
//         break;
//       default:
//         write(STDOUT, "<not supported>", 15);
//       }
//     }
//     else
//     {
//       // No, just a normal character. Print it.
//       write(STDOUT, &format[index], 1);
//     }
//     index++;
//   }

//   // Finish handling variadic arguments
//   va_end(args);
// }
