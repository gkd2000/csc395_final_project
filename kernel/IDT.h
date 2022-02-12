#pragma once
#include <stdint.h>
#include <stddef.h>

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

// The IDT table
static idt_entry_t idt[256];

/**
 * Initialize an interrupt descriptor table, set handlers for standard exceptions and keyboard events, 
 * and install the IDT.
 */
void idt_setup();
