#pragma once
#include <stdint.h>
#include <stddef.h>

// #include "handler.h"







// A struct the matches the layout of an IDT entry
typedef struct idt_entry {
  uint16_t offset_0;
  uint16_t selector;
  uint8_t ist : 3;
  uint8_t _unused_0 : 5;
  uint8_t type : 4;
  uint8_t _unused_1 : 1;
  uint8_t dpl : 2;
  uint8_t present : 1;
  uint16_t offset_1;
  uint32_t offset_2;
  uint32_t _unused_2;
} __attribute__((packed)) idt_entry_t;



static idt_entry_t idt[256];


// memset takes in any pointer and the size of the memory that the pointer points to.
// it will set all the memories to 0, so it does not have random data.
// we assume that the pointer is a multiple 1 byte.
// it set all the entries to c, which is a parameter
void memset(void* arr, int c, uint32_t size);


void idt_setup();
