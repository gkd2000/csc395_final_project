#pragma once

#include <stdint.h>

// Standard port input/output operations
// Source: https://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access

static inline void outb(uint16_t port, uint8_t val) {
  __asm__("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline uint16_t inw(uint16_t port) {
  uint16_t ret;
  __asm__("inw %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline uint32_t inl(uint16_t port) {
  uint32_t ret;
  __asm__("inl %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline void io_wait() {
  outb(0x80, 0);
}