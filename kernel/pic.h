#pragma once

#include <stdint.h>
#include <stddef.h>

#include "port.h"

// Ports to talk to PICs
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

// Message sent to a PIC when finished with an IRQ
#define PIC_EOI 0x20

// Interrupt numbers for IRQs
#define IRQ0_INTERRUPT 0x20
#define IRQ1_INTERRUPT 0x21
#define IRQ2_INTERRUPT 0x22
#define IRQ3_INTERRUPT 0x23
#define IRQ4_INTERRUPT 0x24
#define IRQ5_INTERRUPT 0x25
#define IRQ6_INTERRUPT 0x26
#define IRQ7_INTERRUPT 0x27
#define IRQ8_INTERRUPT 0x28
#define IRQ9_INTERRUPT 0x29
#define IRQ10_INTERRUPT 0x2a
#define IRQ11_INTERRUPT 0x2b
#define IRQ12_INTERRUPT 0x2c
#define IRQ13_INTERRUPT 0x2d
#define IRQ14_INTERRUPT 0x2e
#define IRQ15_INTERRUPT 0x2f

/// Initialize the PICs to pass IRQs starting at 0x20
void pic_init();

/// Mask an IRQ by number (0-15)
void pic_mask_irq(uint8_t num);

/// Unmask an IRQ by number (0-15)
void pic_unmask_irq(uint8_t num);