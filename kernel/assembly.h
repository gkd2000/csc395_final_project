#pragma once
#include <stdint.h>
#include <stddef.h>

uint64_t read_cr0();

void write_cr0(uint64_t value);

uintptr_t read_cr3();
