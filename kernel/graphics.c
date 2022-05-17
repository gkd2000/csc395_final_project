#include "graphics.h"

/**
 * Set the color of a single pixel
 * Note that for specifying position, the coordinate system starts at (0, 0) in the top left corner of the screen.
 * The x-component increases as you move rightwards across the screen.
 * The y-component increases as you move down the screen.
 * \param x_pos x-coordinate (in pixels) of the pixel
 * \param y_pos y-coordinate (in pixels) of the pixel
 * \param r     red component of the color
 * \param g     green component of the color
 * \param b     blue component of the color
 */
void draw_pixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b) {
  // Get the start of the framebuffer
  unsigned char* pixel = (unsigned char*) global_framebuffer->framebuffer_addr;

  // Print one pixel
  // Multiply the x-component by the number of bytes used to encode each pixel (for us, 3). Multiply the y-component
  // by the number of pixels in a row. Then access the framebuffer like a regular (1-dimensional) array
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch)] = b;
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch) + 1] = g;
  pixel[x_pos * (global_framebuffer->framebuffer_bpp / 8) + (y_pos * global_framebuffer->framebuffer_pitch) + 2] = r;
}

/**
 * Based on code from https://wiki.osdev.org/Drawing_In_a_Linear_Framebuffer
 * Draw a rectangle with specified position, dimension, and color
 * Note that for specifying position, the coordinate system starts at (0, 0) in the top left corner of the screen.
 * The x-component increases as you move rightwards across the screen.
 * The y-component increases as you move down the screen.
 * \param x_pos  x-coordinate (in pixels) of the top left corner of the rectangle
 * \param y_pos  y-coordinate (in pixels) of the top left corner of the rectangle
 * \param width  width of the rectangle in pixels
 * \param height height of the rectangle in pixels 
 * \param color  color of the rectangle as a hexadecimal color code
 */ 
void kdraw_rectangle(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height, uint32_t color) {

  // If the position is out of bounds, return immediately
  if(x_pos >= global_framebuffer->framebuffer_width || y_pos >= global_framebuffer->framebuffer_height) {
    return;
  }

  // Get some information about the framebuffer
  unsigned char* pixel = (unsigned char*) global_framebuffer->framebuffer_addr;
  uint8_t pixelwidth = global_framebuffer->framebuffer_bpp / 8;
  uint16_t pitch = global_framebuffer->framebuffer_pitch;

  // Decompose hexadecimal color into red, green, and blue components
  uint8_t red = (color & 0xFF0000) >> 16;
  uint8_t green = (color & 0x00FF00) >> 8;
  uint8_t blue = color & 0x0000FF;

  // Truncate the rectangle if necessary to be completely on the screen
  if(x_pos + width >= global_framebuffer->framebuffer_width) {
    width = global_framebuffer->framebuffer_width - x_pos;
  }
  if(y_pos + height >= global_framebuffer->framebuffer_height) {
    height = global_framebuffer->framebuffer_height - y_pos;
  }

  // Draw the rectangle. Outer loop advances down the rows, inner loop advances across columns
  for(int y = y_pos * pitch; y < (y_pos + height) * pitch; y += pitch) {
    for(int x = x_pos * pixelwidth; x < (x_pos + width) * pixelwidth; x += pixelwidth) {
      pixel[x+y] = blue;
      pixel[x+y+1] = green;
      pixel[x+y+2] = red;
    }
  }
}

/**
 * Sets all pixels on the screen to black except the cursor
 */ 
void clear_screen() {

  // Get some information about the framebuffer
  unsigned char* pixel = (unsigned char*) global_framebuffer->framebuffer_addr;
  uint8_t pixelwidth = global_framebuffer->framebuffer_bpp / 8;
  uint16_t pitch = global_framebuffer->framebuffer_pitch;

  // Draw the rectangle. Outer loop advances down the rows, inner loop advances across columns
  for(int y = 0; y < global_framebuffer->framebuffer_height * pitch; y += pitch) {
    for(int x = 0; x < global_framebuffer->framebuffer_width * pixelwidth; x += pixelwidth) {
      pixel[x+y] = 0;
      pixel[x+y+1] = 0;
      pixel[x+y+2] = 0;
    }
  }

  // Clear the saved pixels array so that we don't save background we don't want
  update_saved_pixels(0x000000);

  // Redraw the cursor 
  kdraw_rectangle(data->x_pos, data->y_pos, CURSOR_WIDTH, CURSOR_HEIGHT, WHITE);
}
