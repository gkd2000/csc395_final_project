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

// TODO: Stopped just before Re-enabling System Calls
//  need to set up the TSS and maybe do more things after that

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
  // set_term_write((term_write_t)tag->term_write);
}


void _start(struct stivale2_struct *hdr) {
  // We've booted! Let's start processing tags passed to use from the bootloader
  term_setup(hdr);

  idt_setup();
  gdt_setup();

  // Find the start of higher half direct map (virtual memory)
  struct stivale2_struct_tag_hhdm *virtual = find_tag(hdr, STIVALE2_STRUCT_TAG_HHDM_ID);

  // Get information about physical memory
  struct stivale2_struct_tag_memmap *physical = find_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);

  // Set up the free list and enable write protection
  freelist_init(virtual, physical);
  term_init();
  uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;

  pic_init();
  pic_unmask_irq(1);

  unmap_lower_half(root);

  // Get information about the modules we've asked the bootloader to load
  struct stivale2_struct_tag_modules *modules = find_tag(hdr, STIVALE2_STRUCT_TAG_MODULES_ID);
  module_setup(modules);
  // Test page for init
  uintptr_t test_page = 0x400000000;
  vm_map(read_cr3() & 0xFFFFFFFFFFFFF000, test_page, true, true, false);

  // char buffer1[10];
  // write(1, "hellowor\n", 9);
  // read(0, buffer1, 9);

  // kprintf("%s\n", buffer1);

  // test module
  // kprintf("modules:\n");
  for (int i = 0; i < modules->module_count; i++)
  {
    // kprintf("        %s:\n            %p-%p\n", modules->modules[i].string, modules->modules[i].begin, modules->modules[i].end);
    run_program(modules->modules[i].begin);
  }

  // Test strsep
  // char* token;
  char string[12] = "abc,def,ghi";
  char* str_ptr = (char*) string;

  // while ((token = strsep(&str_ptr, ",")) != NULL) {
  //   kprintf("%s\n", token);
  // }

  // Test strcpy and strncpy
  char des[15];
  strncpy(des, str_ptr, 4);
  for(int i = 0; i < 10; i++) {
    kprintf("%c ", des[i]);
  }
  kprintf("\n");
  kprintf("%s\n", strcpy(des, str_ptr));
  kprintf("%s\n", des);

  kprintf("%s\n", strncpy(des, str_ptr, 14));

  // Test strpbrk
  kprintf("%s\n", strpbrk("abc,def,ghi", ","));
  kprintf("%s\n", strpbrk("abc,def,ghi", " ") ? "true" : "false"); // No occurences
  kprintf("%s\n", strpbrk("abc,def,ghi", "i")); // Last char
  kprintf("%s\n", strpbrk("abc,def,ghi", "a")); // First char
  kprintf("%s\n", strpbrk("abc,def,ghi", "zyxf")); // Several delimiters

  if(strcmp("hi", "bye") <= 0) {
    kprintf("1 failed\n");
  } else {
    kprintf("1 success\n");
  }

  if(strcmp("bye", "hi") >= 0) {
    kprintf("2 failed\n");
  } else {
    kprintf("2 success\n");
  }

  if(strcmp("hi", "hire") >= 0) {
    kprintf("3 failed\n");
  } else {
    kprintf("3 success\n");
  }

  if(strcmp("hello world", "hello world") != 0) {
    kprintf("4 failed\n");
  } else {
    kprintf("4 success\n");
  }

  // Test getline
  // char buffer[10];
  // char* buffer_ptr = (char*) buffer;
  // size_t max = 10;
  // size_t chars_read = getline(&buffer_ptr, &max, 0);
  // for(size_t i = 0; i < chars_read; i++) {
  //   kprintf("%c", buffer[i]);
  // }

  // translate(((read_cr3() >> 12) << 12), _start);
  // translate(((read_cr3() >> 12) << 12), NULL);

  // // while(1) {
  // //   kprintf("%c\n", kgetc());
  // // }


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

  // // char arr[100];
  // // kgets(arr, 100);

  // // kprintf(": %s\n", arr);
  // // We're done, just hang...

  // char* buf = "12345";
  // long rc = syscall(SYS_write, 1, buf, 5);
  // if (rc <= 0)
  // {
  //   kprintf("write failed\n");
  // }
  // else
  // {
  //   buf[rc] = '\0';
  //   kprintf("write '%s'\n", buf);
  // }

  // char buff[6];
  // long rc2 = syscall(SYS_read, 0, buff, 5);
  // if (rc2 <= 0)
  // {
  //   kprintf("read failed\n");
  // }
  // else
  // {
  //   buff[rc2] = '\0';
  //   kprintf("read '%s'\n", buff);
  // }

  halt();
}
