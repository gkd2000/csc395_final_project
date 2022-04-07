#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>


void _start() {

  write(1, "$ ", 2);
  int size = 100;
  char buffer[size+1];

  size = getline(buffer, &size, STDIN);
  // read(STDIN, buffer, 5);
  buffer[size] = '\0';
  write(1, buffer, size);
  exec(buffer, NULL);
  exit(0);
}
