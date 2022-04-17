#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>


void _start() {
  char *test_page = (char *)0x600000000;
  test_page[0] = 'h';
  test_page[1] = 'e';
  test_page[2] = 'l';
  test_page[3] = 'l';
  test_page[4] = 'o';
  test_page[5] = '\n';
  syscall(SYS_write, 1, test_page, 6);
  exec("shell", NULL);
  exit(0);
}
