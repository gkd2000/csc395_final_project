#include <stdint.h>
#include <stddef.h>

#include "stivale2.h"
#include "util.h"

// Reserve space for the stack
static uint8_t stack[8192];

// Request a terminal from the bootloader
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0},
    .flags = 0};

// Declare the header for the bootloader
__attribute__((section(".stivale2hdr"), used)) static struct stivale2_header stivale_hdr = {
    // Use ELF file's default entry point
    .entry_point = 0,

    // Use stack (starting at the top)
    .stack = (uintptr_t)stack + sizeof(stack),

    // Bit 1: request pointers in the higher half
    // Bit 2: enable protected memory ranges (specified in PHDR)
    // Bit 3: virtual kernel mappings (no constraints on physical memory)
    // Bit 4: required
    .flags = 0x1E,

    // First tag struct
    .tags = (uintptr_t)&terminal_hdr_tag};

// Find a tag with a given ID
void *find_tag(struct stivale2_struct *hdr, uint64_t id)
{
  // Start at the first tag
  struct stivale2_tag *current = (struct stivale2_tag *)hdr->tags;

  // Loop as long as there are more tags to examine
  while (current != NULL)
  {
    // Does the current tag match?
    if (current->identifier == id)
    {
      return current;
    }

    // Move to the next tag
    current = (struct stivale2_tag *)current->next;
  }

  // No matching tag found
  return NULL;
}

typedef void (*term_write_t)(const char *, size_t);
term_write_t term_write = NULL;

void term_setup(struct stivale2_struct *hdr)
{
  // Look for a terminal tag
  struct stivale2_struct_tag_terminal *tag = find_tag(hdr, STIVALE2_STRUCT_TAG_TERMINAL_ID);

  // Make sure we find a terminal tag
  if (tag == NULL)
    halt();

  // Save the term_write function pointer
  term_write = (term_write_t)tag->term_write;
}

void kprint_c(char c)
{
  term_write(&c, 1);
}

uint32_t count(const char *str)
{
  uint32_t length = 0;
  char *temp = str;
  while (*temp != '\0')
  {
    length++;
    temp++;
  }

  return length;
}
void kprint_s(const char *str)
{
  uint32_t length = count(str);
  term_write(str, length);
}

void kprint_d(uint64_t value)
{
  if (value != 0)
  {
    kprint_d(value / 10);

    kprint_c(value % 10 + 48);
  }
}

void kprint_x(uint64_t value)
{
  char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  if (value != 0)
  {
    kprint_x(value / 16);

    kprint_c(table[value % 16]);
  };
}

void kprint_p(void *ptr)
{
  term_write("0x", 2);
  kprint_x((uint64_t)ptr);
}

void _start(struct stivale2_struct *hdr)
{
  // We've booted! Let's start processing tags passed to use from the bootloader
  term_setup(hdr);

  // find physical memory
  struct stivale2_struct_tag_hhdm *physicalmem = find_tag(hdr, STIVALE2_STRUCT_TAG_HHDM_ID);
  kprint_x(physicalmem->addr);

  struct stivale2_struct_tag_memmap *vir = find_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);
  term_write("\n", 1);

  kprint_x(vir->memmap[0].base);
  term_write("\n", 1);

  // struct stivale2_struct_tag_kernel_base_address *test = find_tag(hdr, 0x060d78874a2a8af0);
  // term_write("physical: ", 10);
  // kprint_x(test->physical_base_address);

  // term_write("physical: ", 10);
  // kprint_x(test->virtual_base_address);

  for (int i = 0; i < vir->entries; i++) {
    if (vir->memmap[i].type == 1) {

      kprint_s("0x");
      kprint_x(vir->memmap[i].base);
      kprint_s("-0x");
      kprint_x(vir->memmap[i].base + vir->memmap[i].length);

      kprint_s(" at 0x");
      kprint_x(vir->memmap[i].base + physicalmem->addr);
      kprint_s("-0x");
      kprint_x(vir->memmap[i].base + vir->memmap[i].length + physicalmem->addr);
      kprint_c('\n');
    }
  }

  // Print a greeting

  // char *a = "Asdasdasd ";
  // kprint_s(a);

  // uint64_t i = 153123123;
  // kprint_x(i);
  // kprint_p(a);
  // We're done, just hang...
  halt();
}
