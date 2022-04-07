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

  getline(buffer, &size, STDIN);
  // read(STDIN, buffer, 5);
  buffer[5] = '\0';
  write(1, buffer, 5);
  write(1, "\n", 1);
  exec(buffer, NULL);
  exit(0);
}
