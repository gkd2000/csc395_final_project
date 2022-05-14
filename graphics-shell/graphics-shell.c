#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>
#include <ugraphics.h>

/**
 * Display the programs we can run (hardcoded in for now), along with the keypress
 * used to launch the program. When the user presses the appropriate key, launch the 
 * requested program.
 */
void _start() {

  // Display programs that run in graphical mode
  gwrite(10, 10, WHITE, "1 - helloworld", 14);
  gwrite(10, 18, WHITE, "2 - paint", 9);

  // Buffer for user input
  char buffer[1];

  // Loop until the user launches a program
  while(true) {
    // Get user input
    read(STDIN, buffer, 1);

    // Process the keypress
    switch(buffer[0]) {
      case '1':
        gexec("helloworld", NULL);
        break;
      case '2':
        gexec("paint", NULL);
        break;
      default:
        break;
    }
  }
  exit(0);
}
