#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <syscall.h>

int64_t drawpixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b);

int64_t readmouse(uintptr_t mouse_data);
