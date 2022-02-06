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

__attribute__((interrupt)) void reserve_handler(interrupt_context_t *ctx);

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

__attribute__((interrupt)) void bound_range_exceeded_handler(interrupt_context_t *ctx);

__attribute__((interrupt)) void invalid_opcode_handler(interrupt_context_t *ctx);

__attribute__((interrupt)) void device_not_available_handler(interrupt_context_t *ctx);

__attribute__((interrupt)) void double_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec);

__attribute__((interrupt)) void coprocessor_segment_overrun_handler_ec(interrupt_context_t *ctx);

__attribute__((interrupt)) void invalid_tss_handler_ec(interrupt_context_t *ctx, uint64_t ec);

__attribute__((interrupt)) void segment_not_present_handler_ec(interrupt_context_t *ctx, uint64_t ec);

__attribute__((interrupt)) void stack_segment_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec);

__attribute__((interrupt)) void general_protection_handler_ec(interrupt_context_t *ctx, uint64_t ec);

__attribute__((interrupt)) void page_fault_handler_ec(interrupt_context_t *ctx, uint64_t ec);

__attribute__((interrupt)) void x87_fpu_floating_point_error_handler(interrupt_context_t *ctx);

__attribute__((interrupt)) void alignment_check_handler(interrupt_context_t *ctx, uint64_t ec);

__attribute__((interrupt)) void machine_check_handler(interrupt_context_t *ctx);

__attribute__((interrupt)) void simd_floating_point_exception_handler(interrupt_context_t *ctx);

__attribute__((interrupt)) void virtualization_exception_handler(interrupt_context_t *ctx);

__attribute__((interrupt)) void control_protection_exception_handler_ec(interrupt_context_t *ctx, uint64_t ec);
