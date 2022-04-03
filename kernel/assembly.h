#pragma once
#include <stdint.h>
#include <stddef.h>

// Read the value of control register zero (used to enable write protection)
uint64_t read_cr0();

// Write to control register zero (used to enable write protection)
void write_cr0(uint64_t value);

// Read the value of control register 3, which stores the pointer to the top
// level page table.
uintptr_t read_cr3();

// Write to control register 3, which stores the pointer to the top level page table
void write_cr3(uint64_t value);
