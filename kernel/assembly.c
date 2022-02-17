#include "assembly.h"

uint64_t read_cr0()
{
  uintptr_t value;
  __asm__("mov %%cr0, %0"
          : "=r"(value));
  return value;
}

void write_cr0(uint64_t value)
{
  __asm__("mov %0, %%cr0"
          :
          : "r"(value));
}

uintptr_t read_cr3()
{
  uintptr_t value;
  __asm__("mov %%cr3, %0"
          : "=r"(value));
  return value;
}
