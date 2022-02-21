#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "stivale2.h"
#include "kprint.h"
#include "assembly.h"

#define SERIAL_NUMBER 9123213123
#define PAGE_SIZE 4096

static uint64_t virtual_offset;

// Format for the entry in a page table
typedef struct page_table {
  uint8_t present : 1;
  uint8_t writable : 1;
  uint8_t kernel : 1;
  uint8_t _unused_0 : 2;
  uint8_t accessed : 1;
  uint8_t _unused_1 : 1;
  uint8_t ps : 1;
  uint8_t _unused_2 : 4;
  uint64_t physical_addr : 51;
  uint8_t no_execute : 1;
} __attribute__((packed)) page_table_t;

// A node in a free list
typedef struct node {
  uint64_t serial_number;
  struct node* next;
} node_t;

/**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 * \param arr  pointer to the memory to be written to
 * \param c    value to be written
 * \param size number of bytes to be written
 * \return arr (the first argument)
 */
void* memset(void *arr, int c, uint32_t size);

/**
 * Does not currently print the usable memory, as reported by the bootloader.
 * Enables write protection and creates the freelist
 * \param virtual  the stivale2 struct tag which gives the beginning of the HHDM
 * \param physical the stivale2 memory map struct tag, which reports the memory map
 *                 the memory map built by the bootloader
 */
void get_usable_memory(struct stivale2_struct_tag_hhdm *virtual, struct stivale2_struct_tag_memmap *physical);

/**
 * Translate a virtual address to its mapped physical address
 *
 * \param address     The virtual address to translate
 */
void translate(uintptr_t page_table, void *address);

/**
 * Allocate a page of physical memory.
 * \returns the physical address of the allocated physical memory or 0 on error.
 */
uintptr_t pmem_alloc();

/**
 * Free a page of physical memory.
 * \param p is the physical address of the page to free, which must be page-aligned.
 */
void pmem_free(uintptr_t p);

/**
 * Map a single page of memory into a virtual address space.
 * \param root       The physical address of the top-level page table structure
 * \param address    The virtual address to map into the address space, must be page-aligned
 * \param user       Should the page be user-accessible?
 * \param writable   Should the page be writable?
 * \param executable Should the page be executable?
 * \returns true if the mapping succeeded, or false if there was an error
 */
bool vm_map(uintptr_t root, uintptr_t address, bool user, bool writable, bool executable);
