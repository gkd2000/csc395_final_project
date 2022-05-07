#pragma once

#include <stdint.h>

#include "stivale-hdr.h"

#define WHITE 0xFFFFFF

void draw_pixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b);