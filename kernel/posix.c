#include "posix.h"

// Keep track of virtual memory handed out by mmap
intptr_t addre = 0x60000000;

// Place to store the modules loaded by the bootloader
static struct stivale2_struct_tag_modules* modules_list;

// Save information about the modules loaded by the bootloader in a global variable
void module_setup(struct stivale2_struct_tag_modules *modules) {
  modules_list = modules;
}

/**
 * Write to size characters from buffer to fd
 * \param fd     the location to write to. Must be 1 (stdout) or 2 (stderr)
 * \param buffer string of characters to write
 * \param size   number of characters to be written
 * \return -1 on failure, number of characters written to fd on success
 */
int64_t sys_write(uint64_t fd, intptr_t buffer, uint64_t size) {
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
 * Mimics functionality of C standard library read function.
 * Reads size bytes of data from the location referenced by fd
 * into buffer.
 * \param fd     the location to read from. Must be 0 (standard input) or 3 (standard input, non-blocking)
 * \param buffer location to store bytes read from filedes
 * \param size   number of bytes to read
 * \returns the number of bytes read (possibly 0 for non-blocking file descriptor), or -1 on error
 */
int64_t sys_read(uint64_t fd, intptr_t buffer, uint64_t size) {
  if (fd != STDIN && fd != STDIN_NONBLOCKING) {
    // invalid file descriptor
    return -1;
  }

  // Determine whether the call is supposed to blocking
  bool blocking;
  if(fd == STDIN) {
    blocking = true;
  } else {
    blocking = false;
  }

  // Read a character (this is where blocking happens if it was requested)
  char c = kgetc(blocking);
  char* output = (char *) buffer;
  size_t length = 0;
  output[length++] = c;
  // Read size characters from stdin
  while (length < size) {
    c = kgetc(blocking);
    if (c == BACKSPACE) {
      // We read a backspace, change our index to overwrite the last character stored
      length = length ? length - 1 : 0;
    } else {
      output[length++] = c;
    }
  }
  return length;
}

/**
 * A pared-down version of the C standard library mmap.
 * Maps pages of memory starting at the beginning of the page containing addr
 * and continuing for at most len bytes. If addr is NULL, then the kernel picks
 * a place to start the mapping. Pages are mapped as user-readable; other protections
 * must be specified.
 * \param addr   an address on the first page to be mapped, or NULL if the kernel should pick
 * \param len    number of bytes to map
 * \param prot   protections for the mapped pages: all pages are marked user-readable.
 *               If prot & 0x10 = 1, then the page(s) will be writable.
 *               If prot & 0x100 = 1, then the page(s) will be executable.
 * \param flags  unused
 * \param fd     unused
 * \param offset unused
 * \returns the (virtual) address of the beginning of the first page mapped
 */
int64_t sys_mmap(void *addr, size_t len, int prot, int flags, int fd, size_t offset) {
  uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
  if (addr == NULL) {
    addr = (void*) addre;
    // Increment the virtual freelist
    addre += (len / PAGE_SIZE + 1) * PAGE_SIZE;
  }

  // Find the start and number of pages for the mapping
  intptr_t addr_start_page = (((intptr_t)addr) & 0xFFFFFFFFFFFFF000);
  intptr_t number_of_pages = (((intptr_t)addr) + len - addr_start_page) / PAGE_SIZE + 1;

  // Get protections
  int write = prot & 2;
  int exec = prot & 4;

  for (int j = 0; j < number_of_pages; j++) {
    // Map one page
    if (!vm_map(root, addr_start_page + j * PAGE_SIZE, 1, write, exec)) {
      kprintf("mem map failed\n");
    }
  }
  return addr_start_page;
}

/**
 * Load and execute a program specified by file.
 * \param file the program to execute. Must be a null-terminated string
 * \param argv unused
 * \returns -1 if no module matching file was found.
 *    Otherwise, an executable will be run, and so this function should not return.
 */
int64_t sys_exec(char* file, char* argv[]) {
  // Loop over modules looking for file
  for (int i = 0; i < modules_list->module_count; i++) {
    if (!strcmp(modules_list->modules[i].string, file)) {
      // We found the program specified! Unmap the lower half of memory
      uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
      unmap_lower_half(root);
      // Load and run the program specified by file.
      run_program(modules_list->modules[i].begin);
      return 0;
    }
  }
  // We didn't find the module
  return -1;
}

/**
 * Clean up after an executable has finished running, and launch the init program.
 * \param status unused
 * \returns status, to match the C standard library exit() system call signature
 */
int64_t sys_exit(int status) {
  // Loop over modules looking for the init program
  for (int i = 0; i < modules_list->module_count; i++) {
    if (!strcmp(modules_list->modules[i].string, "init")) {
      // We found it! Unmap the lower half of memory
      uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
      unmap_lower_half(root);
      // Run init
      run_program(modules_list->modules[i].begin);
    }
  }
  return status;
}

/**
 * Draw a pixel at the specified location on the screen
 * \param x_pos x-coordinate (in pixels) of the pixel
 * \param y_pos y-coordinate (in pixels) of the pixel
 * \param r     red component of the color
 * \param g     green component of the color
 * \param b     blue component of the color
 * \returns 1
 */
int64_t sys_drawpixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b) {
  draw_pixel(x_pos, y_pos, r, g, b);

  return 1;
}

/**
 * Get the most recent data from the mouse
 * \param mouse_data struct (of type mouse_data_t) to populate with recent data
 * \returns 1
 */
int64_t sys_readmouse(uintptr_t mouse_data) {
  mouse_data_t* src_mdata = (mouse_data_t*) mouse_data;

  //Copy updated mouse data from kernel version to user version
  src_mdata->x_pos = data->x_pos;
  src_mdata->y_pos = data->y_pos;
  src_mdata->left_click = data->left_click;
  src_mdata->right_click = data->right_click;
  src_mdata->middle_click = data->middle_click;

  return 1;
}

/**
 * Update the array which stores what will be drawn in place of the cursor once the cursor is moved. 
 * Set all values of this array to be one color. Thus, the cursor leaves a trail of the specified 
 * color behind it.
 * \param color hexadecimal color code for the color to fill the array with
 * \returns 1
 */
int64_t sys_update_cursor_background(int32_t color) {
  update_saved_pixels(color);
  // restore_background = false;
  return 1;
}

/**
 * In graphics mode, write a specified number of characters from buffer to the screen at the requested position.
 * \param x_pos  x-coordinate (in pixels) of the top left corner of the beginning of the string
 * \param y_pos  y-coordinate (in pixels) of the top left corner of the beginning of the string
 * \param color  hexadecimal color code specifying the color of the text
 * \param buffer array of characters to print
 * \param size   number of characters to print. Must be less than the number of characters in buffer
 * \returns 1
 */
int64_t sys_gwrite(uint32_t x_pos, uint32_t y_pos, uint32_t color, void* buffer, size_t size) {
  char* arr = (char*) buffer;

  for(size_t i = 0; i < size; i++) {
    gkprint_c(arr[i], x_pos + (8*i), y_pos, color);
  }

  return 1;
}

/**
 * In graphics mode, make the screen all black and then load and execute a program specified by file.
 * \param file the program to execute. Must be a null-terminated string
 * \param argv unused
 * \returns -1 if no module matching file was found.
 *    Otherwise, an executable will be run, and so this function should not return.
 */
int64_t sys_gexec(char* file, char* argv[]) {

  // Clear the screen (set all pixels to black)
  clear_screen();

  // Run exec as usual
  return sys_exec(file, argv);
}
