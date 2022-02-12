#include "mem.h"

/**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 * \param arr  pointer to the memory to be written to
 * \param c    value to be written
 * \param size number of bytes to be written
 * \return arr (the first argument)
 */
void* memset(void *arr, int c, uint32_t size) {
  
  // Walk through each byte of arr, setting the value to c
  uint8_t *temp = arr;
  for (int i = 0; i < size; i++) {
    temp[i] = c;
  }
  return arr;
}

/**
 * Prints the usable memory, as reported by the bootloader.
 * \param virtual  the stivale2 struct tag which gives the beginning of the HHDM
 * \param physical the stivale2 memory map struct tag, which reports the memory map
 *                 the memory map built by the bootloader
 */
void get_usable_memory(struct stivale2_struct_tag_hhdm *virtual, struct stivale2_struct_tag_memmap *physical) {
  // Store the start of the HHDM 
  uint64_t virtual_offset = virtual->addr;
  // Loop through physical memory entries, looking for usable memory
  for (int i = 0; i < physical->entries; i++) {
    if (physical->memmap[i].type == 1) {
      // We found usable memory! Print its physical and virtual location
      kprintf("0x%d-0x%d mapped at 0x%d-0x%d\n", physical->memmap[i].base,
              physical->memmap[i].base + physical->memmap[i].length, physical->memmap[i].base + virtual_offset,
              physical->memmap[i].base + physical->memmap[i].length + virtual_offset);
    }
  }
}