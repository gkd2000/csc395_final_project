#include <ugraphics.h>

int64_t drawpixel(uint32_t x_pos, uint32_t y_pos, uint8_t r, uint8_t g, uint8_t b) {
  return syscall(SYS_drawpixel, x_pos, y_pos, r, g, b);
}

int64_t readmouse(uintptr_t mouse_data) {
  return syscall(SYS_readmouse, mouse_data);
}

int64_t update_cursor_background(int32_t color) {
  return syscall(SYS_update_cursor_background, color);
}