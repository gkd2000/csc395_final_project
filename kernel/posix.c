#include "posix.h"

void module_setup(struct stivale2_struct_tag_modules *modules) {
  modules_list = modules;
};

/**
 * Write to size characters from buffer to fd
 * \param fd     the location to write to. Must be 1 (stdout) or 2 (stderr)
 * \param buffer string of characters to write
 * \param size   number of characters to be written
 * \return -1 on failure, number of characters written to fd on success
 */
int64_t sys_write(uint64_t fd, intptr_t buffer, size_t size) {
  if ((fd != 1) && (fd != 2)) {
    // fd was not stdout or stderr
    return -1;
  }

  int count = 0;
  char * ptr = (char *) buffer;
  for (uint64_t i = 0; i < size; i++) {
    kprintf("%c", ptr[i]);
    count++;
  }
  return count;
}

/**
 * Read size characters from fd into buffer
 * \param fd     location to read from. Must be 0 (stdin)
 * \param buffer buffer to store read characters in
 * \param size   number of characters to read
 * \return number of characters read
 */
int64_t sys_read(uint64_t fd, intptr_t buffer, uint64_t size) {
  if (fd) {
    // invalid file descriptor
    return -1;
  }

  char c = kgetc();
  char* output = (char *) buffer;
  size_t length = 0;
  output[length++] = c;
  // Read size characters from stdin
  while (length < size) {
    c = kgetc();
    if (c == BACKSPACE) {
      // We read a backspace, change our index to overwrite the last character stored
      length = length ? length - 1 : 0;
    } else {
      output[length++] = c;
    }
  }
  return length;
}

int64_t sys_mmap(void *addr, size_t len, int prot, int flags, int fd, size_t offset) {
  uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
  for (int j = 0; j < ((((intptr_t)addr) + len - (((intptr_t)addr) & 0xFFFFFFFFFFFFF000)) / PAGE_SIZE + 1); j++)
  {
    if (!vm_map(root, (((intptr_t)addr) & 0xFFFFFFFFFFFFF000) + j * PAGE_SIZE, 1, 1, 1))
    {
      kprintf("mem map failed\n");
    }; // Map the virtual address we need
  }
  return (((intptr_t)addr) & 0xFFFFFFFFFFFFF000);
}

int64_t sys_exec(char * file, char* argv[]) {
  for (int i = 0; i < modules_list->module_count; i++)
  {
    if (!strcmp(modules_list->modules[i].string, file)) {
      uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
      unmap_lower_half(root);
      run_program(modules_list->modules[i].begin);
    }
  }
}

int64_t sys_exit(int status) {
  for (int i = 0; i < modules_list->module_count; i++)
  {
    if (!strcmp(modules_list->modules[i].string, "init"))
    {
      uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
      unmap_lower_half(root);
      run_program(modules_list->modules[i].begin);
    }
  }
  return status;
}


