#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

#include "stivale2.h"
#include "util.h"
#include "kprint.h"
#include "IDT.h"
#include "pic.h"
#include "char.h"
#include "mem.h"
#include "assembly.h"
#include "elf.h"
#include "gdt.h"
#include "posix.h"
#include "stivale-hdr.h"
#include "mouse.h"

// TODO: Finish commenting the print functions.

// INFO: Screen resolution is 1024 pixels in width by 768 pixels in height

// Reserve space for the stack
static uint8_t stack[8192];

// Request that a linear framebuffer from the bootloader
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
  .tag = {
    .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
    .next = 0},
  .framebuffer_width = 0,
  .framebuffer_height = 0,
  .framebuffer_bpp = 0 //Not a multiple of 8, OR 2!
};

// Request that a linear framebuffer from the bootloader
static struct stivale2_header_tag_any_video video_hdr_tag = {
  .tag = {
    .identifier = STIVALE2_HEADER_TAG_ANY_VIDEO_ID,
    .next = (uintptr_t) &framebuffer_hdr_tag},
  .preference = 0
};

// Request that the beginning of memory (address 0x0) be unmapped
static struct stivale2_tag unmap_null_hdr_tag = {
  .identifier = STIVALE2_HEADER_TAG_UNMAP_NULL_ID,
  .next = (uintptr_t) &video_hdr_tag
};

// Request a terminal from the bootloader
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = (uintptr_t)&unmap_null_hdr_tag},
    .flags = 0
};


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
    .tags = (uintptr_t)&unmap_null_hdr_tag};

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
  // set_term_write((term_write_t)tag->term_write);
}

void _start(struct stivale2_struct *hdr) {
  // We've booted! Let's start processing tags passed to use from the bootloader
  // term_setup(hdr);
  global_hdr = hdr;

  

  // Set up the interrupt descriptor table and global descriptor table
  idt_setup();
  gdt_setup();

  // Find the start of higher half direct map (virtual memory)
  struct stivale2_struct_tag_hhdm *virtual = find_tag(hdr, STIVALE2_STRUCT_TAG_HHDM_ID);

  // Get information about physical memory
  struct stivale2_struct_tag_memmap *physical = find_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);

  //Set up framebuffer
  struct stivale2_struct_tag_framebuffer *framebuffer = find_tag(hdr, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
  global_framebuffer = framebuffer;
  
  unsigned char* pixel = (unsigned char*) framebuffer->framebuffer_addr;
  // TODO: Try to map something
  // if(vm_map(read_cr3() & 0xFFFFFFFFFFFFF000, framebuffer->framebuffer_addr + virtual->addr, true, true, false)) {
  //   pixel = (unsigned char*) (framebuffer->framebuffer_addr + virtual->addr);
  // } else {
  //   pixel = (unsigned char*) framebuffer->framebuffer_addr;
  // }

  // Potential way to debug: have an if statement that assigns pixel as on line 130, which crashes
  // the kernel. then we'll know the outcome of the if.

  // This line causes a page fault or something else that restarts the kernel
  // unsigned char* pixel = (unsigned char*) (framebuffer->framebuffer_addr + virtual->addr);

  // unsigned char* pixel = (unsigned char*)0xA0000;


  // for(int i = 0; i < framebuffer->framebuffer_width; i++) {
  //   for(int j = 0; j < framebuffer->framebuffer_height; j++) {
  //     pixel[i * (framebuffer->framebuffer_bpp / 8) + (j * 
  //           (framebuffer->framebuffer_width * (framebuffer->framebuffer_bpp / 8)))] = 2;
  //   }
  // }

  // for(int i = 10; i < 40; i++) {
  //   for(int j = 10; j < 20; j++) {
  //     pixel[i * (framebuffer->framebuffer_bpp / 8) + (j * framebuffer->framebuffer_pitch)] = 0;
  //     pixel[i * (framebuffer->framebuffer_bpp / 8) + (j * framebuffer->framebuffer_pitch) + 1] = 0;
  //     pixel[i * (framebuffer->framebuffer_bpp / 8) + (j * framebuffer->framebuffer_pitch) + 2] = 255;
  //   }
  // }
// hello
  // for(int i = 40; i < 50; i++) {
  //   for(int j = 10; j < 40; j++) {
  //     pixel[i * (framebuffer->framebuffer_bpp / 8) + (j * framebuffer->framebuffer_pitch)] = 0;
  //     pixel[i * (framebuffer->framebuffer_bpp / 8) + (j * framebuffer->framebuffer_pitch) + 1] = 255;
  //     pixel[i * (framebuffer->framebuffer_bpp / 8) + (j * framebuffer->framebuffer_pitch) + 2] = 0;
  //   }
  // }

  // while(1){}

  // unsigned char* location = (unsigned char*)0xA0000 + 320 * 10 /* y */ + 10 /* x */;
  // *location = 4;

  // Set up the free list and enable write protection
  freelist_init(virtual, physical);

  // Initialize the terminal
  term_init();
  
  pic_init();
  pic_unmask_irq(1);
  pic_unmask_irq(2);
  pic_unmask_irq(12);

  //outb(0x64, 0x20);

  // Unmap the lower half of memory
  uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
  unmap_lower_half(root);

  // int red = 255;
  // int green = 255;
  // int blue = 255;
  // for(int i = 0; i < 600; i++) {
  //   for(int j = 0; j < 600; j++) {
  //     if(j % 3 == 0) {
  //       red -= 1;
  //     } else if(j % 3 == 1) {
  //       green -= 1;
  //     } else {
  //       blue -= 1;
  //     }
  //     draw_pixel(i, j, red, green, blue);
  //   }
  // }

  gkprint_c('W', 286, 150, WHITE);
  gkprint_c('H', 286, 158, WHITE);
  gkprint_d(framebuffer->framebuffer_width, 300, 150, WHITE);
  gkprint_d(framebuffer->framebuffer_height, 300, 158, WHITE);

  draw_rectangle(150, 150, 50, 50, 0xFF0000);
  gkprint_c('A', 150, 150, WHITE);
  draw_rectangle(1000, 700, 100, 100, 0x3498eb);
  
  initialize_cursor();
  initialize_mouse();

  // Get information about the modules we've asked the bootloader to load
  struct stivale2_struct_tag_modules *modules = find_tag(hdr, STIVALE2_STRUCT_TAG_MODULES_ID);
  // Save information about the modules to be accessed later when we make an exec system call
  module_setup(modules);

  // Test for mmap (lines 125 - 143). The code generates a page fault if mmap is not called.
  // Map to a specified address
  // char* test_page = (char*) 0x400000000;
  // sys_mmap(test_page, 9000, 3, 0, 0, 0);

  // Let the kernel pick the address to map to
  char *test_page = (char *)sys_mmap(NULL, 9000, 3, 0, 0, 0);

  // Write to the first page mapped
  char *test_str = "test";
  for(int i = 0; i < strlen(test_str); i++) {
    test_page[i] = test_str[i];
  }

  // Write to the second page
  test_str = "mmap";
  for(int i = 0; i < strlen(test_str); i++) {
    test_page[5000+i] = test_str[i];
  }

  // Print what we wrote.
  kprintf("%s %s\n", test_page, &(test_page[5000]));

  // Test kprintf
  char string[12] = "abc,def,ghi";
  kprintf("kprintf test: %c, %s, %d, %x, %p\n", string[0], string, 123, 123, string);

  // Test that user programs really run in user mode.
  // Map some memory in the lower half. Does not generate a page fault when written to in init 
  // (commented lines of init)
  uintptr_t test_page2 = 0x600000000;
  vm_map(read_cr3() & 0xFFFFFFFFFFFFF000, test_page2, true, true, false);

  // Map some memory in the higher half. 
  // Generates a page fault when accessed from user mode (commented lines of helloworld)
  intptr_t higher_half_addr = 0xffff800000001000;
  vm_map(root, higher_half_addr, 0, 1, 0);

  // Launch the init program
  // for (int i = 0; i < modules->module_count; i++) {
  //   if (!strcmp(modules->modules[i].string, "init")) {
  //     run_program(modules->modules[i].begin);
  //   }
  // }

  halt();
}
