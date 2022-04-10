#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>


void _start() {
  
  int size = 100;
  char buffer[size+1];

  while(true) {
    // Prompt for command and read user input
    write(1, "$ ", 2);
    size = getline(buffer, &size, STDIN);
    
    // Overwrite the newline character
    buffer[size-1] = '\0';

    // Print the user input 
    write(STDOUT, buffer, size);
    write(STDOUT, "\n", 1);

    // Try to exec the user input
    if(exec(buffer, NULL) == -1) {
      printf("Invalid command\n");
      // write(STDOUT, "Invalid command\n", 16);
    } else {
      exit(0);
    }
  }
}
