#include "pic.h"

#include <stdint.h>
#include <stddef.h>

#include "port.h"

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10
 
#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_PIC2 0x08
#define ICW4_BUF_PIC1 0x0C
#define ICW4_SFNM 0x10

/**
 * Initialize the PICs to pass IRQs starting at 0x20
 * Based on code from https://wiki.osdev.org/PIC
 */
void pic_init() {
  // Start initializing PICs
  outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();

  // Set offset for primary PIC
  outb(PIC1_DATA, IRQ0_INTERRUPT);
  io_wait();

  // Set offset for secondary PIC
  outb(PIC2_DATA, IRQ8_INTERRUPT);
  io_wait();

  // Tell primary PIC there is a secondary at IRQ2
  outb(PIC1_DATA, 0x04);
  io_wait();

  // Tell secondary PIC its identity
  outb(PIC2_DATA, 0x02);
  io_wait();
 
  // Finish initialization
  outb(PIC1_DATA, ICW4_8086);
  io_wait();
  outb(PIC2_DATA, ICW4_8086);
  io_wait();

  // Mask all IRQs by default
  outb(PIC1_DATA, 0xFF);
  outb(PIC2_DATA, 0xFF);

  // Enable interrupts
  __asm__("sti");
}

/// Mask an IRQ by number (0-15)
void pic_mask_irq(uint8_t num) {
  // Which PIC do we need to talk to?
  if (num < 8) {
    // Get the current mask
    uint8_t mask = inb(PIC1_DATA);

    // Update the mask
    mask |= 1<<num;
    outb(PIC1_DATA, mask);

  } else if (num < 16) {
    // Get the current mask
    uint8_t mask = inb(PIC2_DATA);

    // Update the mask
    mask |= 1<<(num - 8);
    outb(PIC2_DATA, mask);
  }
}

/// Unmask an IRQ by number (0-15)
void pic_unmask_irq(uint8_t num) {
  // Which PIC do we need to talk to?
  if (num < 8) {
    // Get the current mask
    uint8_t mask = inb(PIC1_DATA);

    // Update the mask
    mask &= ~(1<<num);
    outb(PIC1_DATA, mask);

  } else if (num < 16) {
    // Get the current mask
    uint8_t mask = inb(PIC2_DATA);

    // Update the mask
    mask &= ~(1<<(num - 8));
    outb(PIC2_DATA, mask);
  }
}