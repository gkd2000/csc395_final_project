#include "graphics.h"

void draw_pixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b) {
  unsigned char* pixel = (unsigned char*) global_framebuffer->framebuffer_addr;
  //Print one pixel
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch)] = b;
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch) + 1] = g;
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch) + 2] = r;
}