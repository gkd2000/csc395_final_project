#include "mem.h"

/**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 */
void* memset(void *arr, int c, uint32_t size)
{
  // create a enmty idt
  uint8_t *temp = arr;
  for (int i = 0; i < size; i++)
  {
    temp[i] = c;
  }
  return arr;
}
