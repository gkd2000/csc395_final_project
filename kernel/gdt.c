#include "gdt.h"

#define MAX_GDT_SIZE 256

// Reserve space for a GDT that we'll fill in below
uint8_t gdt[MAX_GDT_SIZE];
size_t gdt_size = 0;
// Reserve space for interrupt handlers to use as a stack
uint8_t interrupt_stack[0x8000];

// Struct definition for a segment descriptor
typedef struct seg_descriptor {
  uint16_t limit_0;
  uint32_t base_0 : 24;
  uint8_t access;
  uint8_t limit_1 : 4;
  uint8_t flags : 4;
  uint8_t base_1;
} __attribute__((packed)) seg_descriptor_t;

// Create a code descriptor at the specified offset
void gdt_code_descriptor(uint16_t offset, bool user) {
  // Get a pointer to the new descriptor
  seg_descriptor_t* d = (seg_descriptor_t*)&gdt[offset];
  if (gdt_size < offset + sizeof(seg_descriptor_t)) {
    gdt_size = offset + sizeof(seg_descriptor_t);
  }

  // Zero out the descriptor
  memset(d, 0, sizeof(seg_descriptor_t));

  // Fill in the type and flags fields
  d->access = 0x9A | (user ? 0x60 : 0);
  d->flags = 0x2;
}

// Create a data descriptor at the specified offset
void gdt_data_descriptor(uint16_t offset, bool user) {
  // Get a pointer to the new descriptor
  seg_descriptor_t* d = (seg_descriptor_t*)&gdt[offset];
  if (gdt_size < offset + sizeof(seg_descriptor_t)) {
    gdt_size = offset + sizeof(seg_descriptor_t);
  }

  // Zero out the descriptor
  memset(d, 0, sizeof(seg_descriptor_t));

  // Fill in the type field
  d->access = 0x92 | (user ? 0x60 : 0);
}

typedef struct tss {
  uint32_t reserved_0;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint64_t reserved_1;
  uint64_t ist1;
  uint64_t ist2;
  uint64_t ist3;
  uint64_t ist4;
  uint64_t ist5;
  uint64_t ist6;
  uint64_t ist7;
  uint64_t reserved_2;
  uint16_t reserved_3;
  uint16_t iomap;
} __attribute__((packed)) tss_t;

// Declare a task state segment
tss_t tss;

// Struct definition for a system descriptor
typedef struct sys_descriptor {
  uint16_t limit_0;
  uint32_t base_0 : 24;
  uint8_t access;
  uint8_t limit_1 : 4;
  uint8_t flags : 4;
  uint64_t base_1 : 40;
  uint32_t reserved;
} __attribute__((packed)) sys_descriptor_t;

// Create a TSS descriptor at the specified offset
void gdt_tss_descriptor(uint16_t offset, tss_t* tss) {
  // Get a pointer to the descriptor
  sys_descriptor_t* d = (sys_descriptor_t*)&gdt[offset];
  if (gdt_size < offset + sizeof(sys_descriptor_t)) {
    gdt_size = offset + sizeof(sys_descriptor_t);
  }

  // Zero out the descriptor
  memset(d, 0, sizeof(sys_descriptor_t));

  // The base fields point to the TSS
  d->base_0 = (uintptr_t)tss;
  d->base_1 = (uintptr_t)tss >> 24;

  // The limit records the size of the TSS in bytes
  d->limit_0 = sizeof(tss_t);
  d->limit_1 = sizeof(tss_t) >> 16;
  
  // Set the descriptor type
  d->access = 0x89;
}

typedef struct gdt_record {
  uint16_t sz;
  void* base;
} __attribute__((packed)) gdt_record_t;



void gdt_setup() {
  // Zero out the gdt
  memset(gdt, 0, sizeof(gdt));

  // Create the kernel code and data descriptors
  gdt_code_descriptor(KERNEL_CODE_SELECTOR, false);
  gdt_data_descriptor(KERNEL_DATA_SELECTOR, false);

  // Create the user code and data descriptors
  gdt_code_descriptor(USER_CODE_SELECTOR, true);
  gdt_data_descriptor(USER_DATA_SELECTOR, true);

  // Create a TSS descriptor
  gdt_tss_descriptor(TSS_SELECTOR, &tss);

  // Load the GDT
  gdt_record_t record = {
    .sz = gdt_size - 1,
    .base = gdt
  };
  __asm__("lgdt %0" :: "m"(record));

  // Zero out the TSS
  memset(&tss, 0, sizeof(tss));

  // Interrupts delivered while in user mode should use this stack pointer
  tss.rsp0 = (uintptr_t)interrupt_stack + sizeof(interrupt_stack) - 8;

  // Load the TSS
  __asm__("ltr %%ax" :: "a"(TSS_SELECTOR));
}
