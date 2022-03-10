#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <syscall.h>



void _start() {
  // Issue a write system call
  syscall(SYS_write, 1, "Hello world!\n", 13);

  // Loop forever
  for(;;){}
}
