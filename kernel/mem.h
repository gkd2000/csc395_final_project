#pragma once
#include <stdint.h>
#include <stddef.h>
#include "stivale2.h"
#include "kprint.h"

/**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 * \param arr  pointer to the memory to be written to
 * \param c    value to be written
 * \param size number of bytes to be written
 * \return arr (the first argument)
 */
void* memset(void *arr, int c, uint32_t size);

void get_usable_memory(struct stivale2_struct_tag_hhdm *virtual, struct stivale2_struct_tag_memmap *physical);
