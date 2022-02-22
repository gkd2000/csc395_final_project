#include <stdint.h>
#include <stddef.h>

#include "stivale2.h"
#include "util.h"
#include "kprint.h"
#include "IDT.h"
#include "pic.h"
#include "char.h"
#include "mem.h"
#include "assembly.h"

// Reserve space for the stack
static uint8_t stack[8192];

// Request that the beginning of memory (address 0x0) be unmapped
static struct stivale2_tag unmap_null_hdr_tag = {
  .identifier = STIVALE2_HEADER_TAG_UNMAP_NULL_ID,
  .next = 0
};

// Request a terminal from the bootloader
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = (uintptr_t)&unmap_null_hdr_tag},
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
void *find_tag(struct stivale2_struct *hdr, uint64_t id) {
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


void term_setup(struct stivale2_struct *hdr) {
  // Look for a terminal tag
  struct stivale2_struct_tag_terminal *tag = find_tag(hdr, STIVALE2_STRUCT_TAG_TERMINAL_ID);

  // Make sure we find a terminal tag
  if (tag == NULL)
    halt();

  // Save the term_write function pointer
  set_term_write((term_write_t)tag->term_write);
}


void _start(struct stivale2_struct *hdr) {
  // We've booted! Let's start processing tags passed to use from the bootloader
  term_setup(hdr);
  idt_setup();
  pic_init();
  pic_unmask_irq(1);

  // Find the start of higher half direct map (virtual memory)
  struct stivale2_struct_tag_hhdm *virtual = find_tag(hdr, STIVALE2_STRUCT_TAG_HHDM_ID);

  // Get information about physical memory
  struct stivale2_struct_tag_memmap *physical = find_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);

  get_usable_memory(virtual, physical);

  translate(((read_cr3() >> 12) << 12), _start);
  translate(((read_cr3() >> 12) << 12), NULL);

  // while(1) {
  //   kprintf("%c\n", kgetc());
  // }

  uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
  int *p = (int *)0x500040001231;
  bool result = vm_map(root, (uintptr_t)p, false, true, false);
  if (result)
  {
    *p = 123;
    kprintf("Stored %d at %p\n", *p, p);
  }
  else
  {
    kprintf("vm_map failed with an error\n");
  }

  char arr[100];
  kgets(arr, 100);

  kprintf(": %s\n", arr);
  // We're done, just hang...

  halt();
}
