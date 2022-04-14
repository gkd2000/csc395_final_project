#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>
#include <syscall.h>
#include <unistd.h>

/**
 * Prompt the user for a command (maxumum 100 characters). If
 * the user enters a valid program, then run it. Otherwise,
 * print an error and prompt for another command.
 */
void _start() {

  size_t size = 100;
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
    }
    exit(0);
  }
}
