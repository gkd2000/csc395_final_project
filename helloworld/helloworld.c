#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>


void _start() {


  printf("Hello world %d\n", 12342);
  // Loop forever
  exit(0);
}
