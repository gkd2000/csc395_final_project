#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>
#include <ugraphics.h>


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

  // Clear the screen
  // for(int i = 0; i < SCREEN_HEIGHT; i++) {
  //   for(int j = 0; j < SCREEN_WIDTH; j++) {
  //     drawpixel(j, i, 0, 0, 0);
  //   }
  // }
  
  // Start the shell (in graphics mode)
  gexec("graphics-shell", NULL);
  exit(0);
}
