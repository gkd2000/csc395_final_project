#pragma once

#include <stddef.h>
#include <stdint.h>

void usermode_entry(uint64_t data_sel, uintptr_t stack_ptr, uint64_t code_sel, uintptr_t instruction_ptr, ...);
