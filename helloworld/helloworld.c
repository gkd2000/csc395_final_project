#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>


void _start() {

  // Attempt to write to an address in the higher half. Should generate
  // a page fault when uncommented.
  // char* higher_half_addr = (char*) 0xffff800000001000;
  // higher_half_addr[0] = 'h';

  printf("Hello world %d\n", 12342);
  exit(0);
}
