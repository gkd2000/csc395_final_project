#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>
#include <ugraphics.h>

void _start() {

  // Attempt to write to an address in the higher half. Should generate
  // a page fault when uncommented.
  // char* higher_half_addr = (char*) 0xffff800000001000;
  // higher_half_addr[0] = 'h';

  // Program for text mode
  // printf("Hello world %d\n", 12342);

  //-----------------------------------------------------------------------
  // draw_rectangle(190, 190, 100, 50, WHITE);

  // Graphics mode hello world
  gwrite(200, 200, WHITE, "Hello world!", 12);
  char buffer[1];

  // Loop until the user quits the program (with the 'q' key)
  while(buffer[0] != 'q') {
    read(STDIN, buffer, 1);
  }
  exit(0);
}
