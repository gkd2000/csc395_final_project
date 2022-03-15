#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <syscall.h>
#include <mman.h>



void _start() {
  // Issue a write system call
  int * str = (int*)0x5723842093;
  mmap(str, sizeof(int)*10, 0, 0, 0, 0);
  *str = 5;

  syscall(SYS_write, 1, "Hello world!\n", 13);
  // Loop forever
  for(;;){}
}
