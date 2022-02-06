#include "handler.h"

#include <stdint.h>
#include <stddef.h>

#include "kprint.h"






__attribute__((interrupt))
void example_handler(interrupt_context_t* ctx) {
  kprintf("example interrupt handler\n");
  halt();
}

__attribute__((interrupt)) void reserve_handler(interrupt_context_t *ctx) {
  kprintf("reserved interrupt handler\n");
  halt();
};

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

__attribute__((interrupt))
void bound_range_exceeded_handler(interrupt_context_t *ctx)
{
  kprintf("overflow handler\n");
  halt();
};

__attribute__((interrupt)) void invalid_opcode_handler(interrupt_context_t *ctx) {
  kprintf("invalid opcode handler\n");
  halt();
}

__attribute__((interrupt)) void device_not_available_handler(interrupt_context_t *ctx) {
  kprintf("device not available handler\n");
  halt();
}

__attribute__((interrupt)) void double_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec) {
  kprintf("double fault handler (ec=%d)\n", ec);
  halt();
}

__attribute__((interrupt)) void coprocessor_segment_overrun_handler_ec(interrupt_context_t *ctx) {
  kprintf("coprocessor segment overrun fault handler\n");
  halt();
}

__attribute__((interrupt)) void invalid_tss_handler_ec(interrupt_context_t *ctx, uint64_t ec) {
  kprintf("invalid TSS handler (ec=%d)\n", ec);
  halt();
}

__attribute__((interrupt)) void segment_not_present_handler_ec(interrupt_context_t *ctx, uint64_t ec) {
  kprintf("segment not present handler (ec=%d)\n", ec);
  halt();
}

__attribute__((interrupt)) void stack_segment_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec) {
  kprintf("stack segment fault handler (ec=%d)\n", ec);
  halt();
}

__attribute__((interrupt)) void general_protection_handler_ec(interrupt_context_t *ctx, uint64_t ec) {
  kprintf("general protection handler (ec=%d)\n", ec);
  halt();
}

__attribute__((interrupt)) void page_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec) {
  kprintf("page fault handler (ec=%d)\n", ec);
  halt();
}

__attribute__((interrupt)) void x87_fpu_floating_point_error_handler(interrupt_context_t *ctx) {
  kprintf("x87 fpu floating point error handler\n");
  halt();
}

__attribute__((interrupt)) void alignment_check_handler(interrupt_context_t *ctx, uint64_t ec) {
  kprintf("alignment check handler (ec=%d)\n", ec);
  halt();
}

__attribute__((interrupt)) void machine_check_handler(interrupt_context_t *ctx) {
  kprintf("machine check handler\n");
  halt();
}

__attribute__((interrupt)) void simd_floating_point_exception_handler(interrupt_context_t *ctx)
{
  kprintf("simd floating point exception handler\n");
  halt();
}

__attribute__((interrupt)) void virtualization_exception_handler(interrupt_context_t *ctx) {
  kprintf("virtualization exception handler\n");
  halt();
}

__attribute__((interrupt)) void control_protection_exception_handler_ec(interrupt_context_t *ctx, uint64_t ec) {
  kprintf("control protection exception handler (ec=%d)\n", ec);
  halt();
}


