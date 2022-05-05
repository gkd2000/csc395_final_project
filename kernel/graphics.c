#include "graphics.h"

void draw_pixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b) {
  unsigned char* pixel = (unsigned char*) global_framebuffer->framebuffer_addr;
  //Print one pixel
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch)] = b;
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch) + 1] = g;
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch) + 2] = r;
}

// UNFINISHED
// color is a hexadecimal color
// We stole this from https://wiki.osdev.org/Drawing_In_a_Linear_Framebuffer 
void draw_rectangle(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height, uint32_t color) {
  unsigned char* pixel = (unsigned char*) global_framebuffer->framebuffer_addr;

  uint8_t pixelwidth = global_framebuffer->framebuffer_bpp / 8;
  uint8_t red = color & 0xFF0000;
  uint8_t green = color & 0x00FF00;
  uint8_t blue = color & 0x0000FF;
  for (int x = x_pos*(global_framebuffer->framebuffer_pitch); x < width; x++) {
    for (int y = y_pos; y < height; y++) {
      //putpixel(vram, 64 + j, 64 + i, (r << 16) + (g << 8) + b);
    //   pixel[j*pixelwidth] = r;
    //   where[j*pixelwidth + 1] = g;
    //   where[j*pixelwidth + 2] = b;
    // }
    // where+=pitch;
    }
  }
}
