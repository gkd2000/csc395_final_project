#include "term.h"

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

// Struct representing a single character entry in the VGA buffer
typedef struct vga_entry
{
  uint8_t c;
  uint8_t fg : 4;
  uint8_t bg : 4;
} __attribute__((packed)) vga_entry_t;

// A pointer to the VGA buffer
vga_entry_t *term;

// The current cursor position in the terminal
size_t term_col = 0;
size_t term_row = 0;

// Turn on the VGA cursor
void term_enable_cursor()
{
  // Set starting scaline to 13 (three up from bottom)
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | 13);

  // Set ending scanline to 15 (bottom)
  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
}

// Update the VGA cursor
void term_update_cursor()
{
  uint16_t pos = term_row * VGA_WIDTH + term_col;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Clear the terminal
void term_clear()
{
  // Clear the terminal
  for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
  {
    term[i].c = ' ';
    term[i].bg = VGA_COLOR_BLACK;
    term[i].fg = VGA_COLOR_WHITE;
  }

  term_col = 0;
  term_row = 0;

  term_update_cursor();
}

// Write one character to the terminal
void term_putchar(char c)
{
  // Handle characters that do not consume extra space (no scrolling necessary)
  if (c == '\r')
  {
    term_col = 0;
    term_update_cursor();
    return;
  }
  else if (c == '\b')
  {
    if (term_col > 0)
    {
      term_col--;
      term[term_row * VGA_WIDTH + term_col].c = ' ';
    }
    term_update_cursor();
    return;
  }

  // Handle newline
  if (c == '\n')
  {
    term_col = 0;
    term_row++;
  }

  // Wrap if needed
  if (term_col == VGA_WIDTH)
  {
    term_col = 0;
    term_row++;
  }

  // Scroll if needed
  if (term_row == VGA_HEIGHT)
  {
    // Shift characters up a row
    memcpy(term, &term[VGA_WIDTH], sizeof(vga_entry_t) * VGA_WIDTH * (VGA_HEIGHT - 1));
    term_row--;

    // Clear the last row
    for (size_t i = 0; i < VGA_WIDTH; i++)
    {
      size_t index = i + term_row * VGA_WIDTH;
      term[index].c = ' ';
      term[index].fg = VGA_COLOR_WHITE;
      term[index].bg = VGA_COLOR_BLACK;
    }
  }

  // Write the character, unless it's a newline
  if (c != '\n')
  {
    size_t index = term_col + term_row * VGA_WIDTH;
    term[index].c = c;
    term[index].fg = VGA_COLOR_WHITE;
    term[index].bg = VGA_COLOR_BLACK;
    term_col++;
  }

  term_update_cursor();
}

// Initialize the terminal
void term_init()
{
  // Get a usable pointer to the VGA text mode buffer
  term = (vga_entry_t *)ptov(VGA_BUFFER);

  term_enable_cursor();
  term_clear();
}

void term_write(const char *arr, size_t s) {
  for (int i = 0; i < s; i++) {
    term_putchar(arr[i]);
  }
}
