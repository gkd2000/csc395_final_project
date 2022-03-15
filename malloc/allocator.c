#define _GNU_SOURCE

#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

// The magic number
#define MAGIC_NUM 0xfca1cbd7

// The minimum size returned by malloc
#define MIN_MALLOC_SIZE 16

// Round a value x up to the next multiple of y
#define ROUND_UP(x, y) ((x) % (y) == 0 ? (x) : (x) + ((y) - (x) % (y)))

// The size of a single page of memory, in bytes
#define PAGE_SIZE 0x1000

// A utility logging function that definitely does not call malloc or free
void log_message(char* message);


// this is the struct used to store the information about the page.
typedef struct header {
  int size;
  int magic_num;
} header_t;

// this is the struct used to keep track of the free space.
typedef struct node {
  struct node * next;
} node_t;

// the array of free list.
node_t* freelist[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};


/**
 * Allocate space on the heap.
 * \param size  The minimium number of bytes that must be allocated
 * \returns     A pointer to the beginning of the allocated space.
 *              This function may return NULL when an error occurs.
 */
void* xxmalloc(size_t size) {
  // The entire implementation of this function should be replaced.
  // This is a basic starting point so you can start with a working allocator, but this
  // implementation wastes an enormous amount of space.

  // check whether the size is greater than 2048, if so, just use mmap

  if (size > 2048) {
    void* p = mmap(NULL, ROUND_UP(size, PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    // Check for errors
    if (p == MAP_FAILED) {
      log_message("mmap failed! Giving up.\n");
      exit(2);
    }

    return p;
  }

  // Round the size up to the next multiple of the page size

  // round up the size to the smallest powers of 2.
  int temp = 16;

  // get the index of the sized object.
  int ind = 0;
  while (size > temp && temp < 2048) {
    temp *= 2;
    ind++;
  }

  size = temp;

  // check whether there are free space left, if not, fill the free list first.

  if (freelist[ind] == NULL) {

    // Request memory from the operating system in page-sized chunks
    void* p = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    // Check for errors
    if (p == MAP_FAILED) {
      log_message("mmap failed! Giving up.\n");
      exit(2);
    }

    // split up the page.
    header_t * head = (header_t*)p;
    head->magic_num = MAGIC_NUM;
    head->size = size;

    // chop the chunksize up into multiple pieces, add them to the free list.
    intptr_t address = (intptr_t)p;
    for (int i = 1; i < PAGE_SIZE/size; i++) {
      // add one space to the free list once at a time
      address += size;
      node_t* new_space = (node_t*)address;
      new_space->next = freelist[ind];
      freelist[ind] = new_space;

    }
  }

  // return the free space.
  void * space = freelist[ind];
  freelist[ind] = freelist[ind]->next;
  return space;

}

/**
 * Free space occupied by a heap object.
 * \param ptr   A pointer somewhere inside the object that is being freed
 */
void xxfree(void* ptr) {
  // Don't free NULL!
  if (ptr == NULL) return;

  // TODO: Complete this function
  // get the start of the page, to check for both size and the magic number.
  intptr_t address = (intptr_t) ptr;

  header_t* page = (header_t*)(address - (address % PAGE_SIZE));

  // check whether it is the actual pointer we gave to the user.
  if (page->magic_num != MAGIC_NUM){
    // log_message("Invalid free!\n");
    return;
  }

  int size = page->size;
  // get the index of the sized object.
  int temp = 16;
  int ind = 0;
  while (size > temp && temp < 2048) {
    temp *= 2;
    ind++;
  }

  size = temp;

  // get to the start of the allocated memory.
  address -= (address%size);

  node_t * mem = (node_t*) address;

  // put the memory back to the free list.

  mem->next = freelist[ind];
  freelist[ind] = mem;
  return;
}

/**
 * Get the available size of an allocated object. This function should return the amount of space
 * that was actually allocated by malloc, not the amount that was requested.
 * \param ptr   A pointer somewhere inside the allocated object
 * \returns     The number of bytes available for use in this object
 */
size_t xxmalloc_usable_size(void* ptr) {
  // If ptr is NULL always return zero
  if (ptr == NULL) {
    return 0;
  }

  // TODO: Complete this function. Just return zero for now.

  // get the start of the page, to check for both size and the magic number.
  intptr_t address = (intptr_t) ptr;
  
  header_t* page = (header_t*)(address - (address % PAGE_SIZE));

  if (page->magic_num != MAGIC_NUM){
    return 0;
  }

  return page->size;
}

/**
 * Print a message directly to standard error without invoking malloc or free.
 * \param message   A null-terminated string that contains the message to be printed
 */
void log_message(char* message) {
  // Get the message length
  size_t len = 0;
  while (message[len] != '\0') {
    len++;
  }

  // Write the message
  if (write(STDERR_FILENO, message, len) != len) {
    // Write failed. Try to write an error message, then exit
    char fail_msg[] = "logging failed\n";
    write(STDERR_FILENO, fail_msg, sizeof(fail_msg));
    exit(2);
  }
}
