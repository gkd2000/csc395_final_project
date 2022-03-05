#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SYS_read 0
#define SYS_write 1

extern int syscall(uint64_t nr, ...);

void _start() {
  // Issue a write system call
  syscall(SYS_write, 1, "Hello world!\n", 13);

  // Loop forever
  for(;;){}
}
