#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>


void _start() {

  // Test that a mapping is accessible from user-mode 
  // printf("Test virtual memory and user-mode: ");
  // char *test_page = (char *)0x600000000;
  // test_page[0] = 'h';
  // test_page[1] = 'e';
  // test_page[2] = 'l';
  // test_page[3] = 'l';
  // test_page[4] = 'o';
  // test_page[5] = '\n';
  // write(STDOUT, test_page, 6);

  // Start the shell
  exec("shell", NULL);
  exit(0);
}
