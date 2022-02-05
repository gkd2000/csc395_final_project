#include "handler.h"

#include <stdint.h>
#include <stddef.h>






__attribute__((interrupt))
void example_handler(interrupt_context_t* ctx) {
  kprintf("example interrupt handler\n");
  halt();
}

__attribute__((interrupt))
void divide_error_handler(interrupt_context_t* ctx) {
  kprintf("divide error handler\n");
  halt();
};

__attribute__((interrupt))
void debug_exception_handler(interrupt_context_t* ctx) {
  kprintf("debug exception handler\n");
  halt();
}

__attribute__((interrupt))
void nmi_interrupt_handler(interrupt_context_t* ctx) {
  kprintf("nmi interrupt handler\n");
  halt();
}

__attribute__((interrupt))
void breakpoint_handler(interrupt_context_t* ctx) {
  kprintf("breakpoint handler\n");
  halt();
};

__attribute__((interrupt))
void overflow_handler(interrupt_context_t* ctx) {
    kprintf("overflow handler\n");
  halt();
};