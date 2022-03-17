#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>


void _start() {
  // Issue a write system call

  // // int * str = (int*)0x5723842093;
  // // mmap(str, sizeof(int)*10, 0, 0, 0, 0);
  // // *str = 5;
  // syscall(SYS_write, 1, "Hello world!\n", 13);
  // // syscall(SYS_write, 1, "Hello world!\n", 13);
  // write(1, "Hello world!\n", 13);

  // syscall(SYS_write, 1, "Hello world!\n", 13);
    // Loop forever
  for(;;){}

  char* test_page = (char*)0x400000000;
  test_page[0] = 'h';
  test_page[1] = 'e';
  test_page[2] = 'l';
  test_page[3] = 'l';
  test_page[4] = 'o';
  test_page[5] = '\n';
  syscall(SYS_write, 1, test_page, 6);

  // Loop forever
  for(;;){}
}
