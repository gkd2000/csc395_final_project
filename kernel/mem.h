#pragma once
#include <stdint.h>
#include <stddef.h>
#include "stivale2.h"
#include "kprint.h"

typedef struct page_table
{
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

typedef struct virtual_addr
{
  uint16_t offset : 12;
  uint16_t level1 : 9;
  uint16_t level2 : 9;
  uint16_t level3 : 9;
  uint16_t level4 : 9;
} __attribute__((packed)) virtual_addr_t;

    /**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 * \param arr  pointer to the memory to be written to
 * \param c    value to be written
 * \param size number of bytes to be written
 * \return arr (the first argument)
 */
    void *
    memset(void *arr, int c, uint32_t size);

/**
 * Prints the usable memory, as reported by the bootloader.
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
