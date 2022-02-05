#pragma once
#include <stdint.h>
#include <stddef.h>

#include "util.h"


typedef struct interrupt_context {
  uintptr_t ip;
  uint64_t cs;
  uint64_t flags;
  uintptr_t sp;
  uint64_t ss;
} __attribute__((packed)) interrupt_context_t;


__attribute__((interrupt))
void divide_error_handler(interrupt_context_t* ctx);

__attribute__((interrupt))
void debug_exception_handler(interrupt_context_t* ctx);

__attribute__((interrupt))
void nmi_interrupt_handler(interrupt_context_t* ctx);

__attribute__((interrupt))
void breakpoint_handler(interrupt_context_t* ctx);

__attribute__((interrupt))
void overflow_handler(interrupt_context_t* ctx);