#include "assembly.h"

// Read the value of control register zero (used to enable write protection)
uint64_t read_cr0() {
  uintptr_t value;
  __asm__("mov %%cr0, %0"
          : "=r"(value));
  return value;
}

// Write to control register zero (used to enable write protection)
void write_cr0(uint64_t value) {
  __asm__("mov %0, %%cr0"
          :
          : "r"(value));
}

// Read the value of control register 3, which stores the pointer to the top
// level page table.
uintptr_t read_cr3() {
  uintptr_t value;
  __asm__("mov %%cr3, %0"
          : "=r"(value));
  return value;
}
