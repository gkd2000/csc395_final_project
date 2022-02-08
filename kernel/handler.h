#pragma once
#include <stdint.h>
#include <stddef.h>

// #include "util.h"

// Struct for storing what was happening when an interrupt occurred
typedef struct interrupt_context {
  uintptr_t ip;
  uint64_t cs;
  uint64_t flags;
  uintptr_t sp;
  uint64_t ss;
} __attribute__((packed)) interrupt_context_t;

// Do we need this?
__attribute__((interrupt)) void reserve_handler(interrupt_context_t *ctx); 

// Handler for a divide error fault
__attribute__((interrupt)) void divide_error_handler(interrupt_context_t* ctx);

// Handler for a debug exception (fault/trap)
__attribute__((interrupt)) void debug_exception_handler(interrupt_context_t* ctx);

// Handler for an NMI interrupt
__attribute__((interrupt)) void nmi_interrupt_handler(interrupt_context_t* ctx);

// Handler for a breakpoint trap
__attribute__((interrupt)) void breakpoint_handler(interrupt_context_t* ctx);

// Handler for an overflow trap
__attribute__((interrupt)) void overflow_handler(interrupt_context_t* ctx);

// Handler for a BOUND range exceeded fault
__attribute__((interrupt)) void bound_range_exceeded_handler(interrupt_context_t *ctx);

// Handler for an invalid opcode fault
__attribute__((interrupt)) void invalid_opcode_handler(interrupt_context_t *ctx);

// Handler for a devide not available fault
__attribute__((interrupt)) void device_not_available_handler(interrupt_context_t *ctx);

// Handler for a double fault abort
__attribute__((interrupt)) void double_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec);

// Handler for a coprocessor segment overrun fault
__attribute__((interrupt)) void coprocessor_segment_overrun_handler(interrupt_context_t *ctx);

// Handler for an invalid TSS fault
__attribute__((interrupt)) void invalid_tss_handler_ec(interrupt_context_t *ctx, uint64_t ec);

// Handler for a segment not present fault
__attribute__((interrupt)) void segment_not_present_handler_ec(interrupt_context_t *ctx, uint64_t ec);

// Handler for a stack-segment fault
__attribute__((interrupt)) void stack_segment_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec);

// Handler for a general protection fault
__attribute__((interrupt)) void general_protection_handler_ec(interrupt_context_t *ctx, uint64_t ec);

// Handler for a page fault
__attribute__((interrupt)) void page_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec);

// Handler for an x87 FPU floating-point error (math fault)
__attribute__((interrupt)) void x87_fpu_floating_point_error_handler(interrupt_context_t *ctx);

// Handler for an alignment check fault
__attribute__((interrupt)) void alignment_check_handler_ec(interrupt_context_t *ctx, uint64_t ec);

// Handler for a machine check abort
__attribute__((interrupt)) void machine_check_handler(interrupt_context_t *ctx);

// Handler for an SIMD floating-point exception (fault)
__attribute__((interrupt)) void simd_floating_point_exception_handler(interrupt_context_t *ctx);

// Handler for a virtualization exception (fault)
__attribute__((interrupt)) void virtualization_exception_handler(interrupt_context_t *ctx);

// Handler for a control protection exception (fault)
__attribute__((interrupt)) void control_protection_exception_handler_ec(interrupt_context_t *ctx, uint64_t ec);

// Handler for a key press (irq1)
__attribute__((interrupt)) void irq1_interrupt_handler(interrupt_context_t *ctx);