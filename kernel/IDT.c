#include "IDT.h"

#include "kprint.h"


// Every interrupt handler must specify a code selector. We'll use entry 5 (5*8=0x28), which
// is where our bootloader set up a usable code selector for 64-bit mode.
#define IDT_CODE_SELECTOR 0x28

// IDT entry types
#define IDT_TYPE_INTERRUPT 0xE
#define IDT_TYPE_TRAP 0xF



// Make an IDT

void memset(void* arr, int c, uint32_t size) {
    // create a enmty idt
    uint8_t * temp = arr;
    for (int i = 0; i < size; i++) {
        temp[i] = c;
    }
}




/**
 * Set an interrupt handler for the given interrupt number.
 *
 * \param index The interrupt number to handle
 * \param fn    A pointer to the interrupt handler function
 * \param type  The type of interrupt handler being installed.
 *              Pass IDT_TYPE_INTERRUPT or IDT_TYPE_TRAP from above.
 */
void idt_set_handler(uint8_t index, void* fn, uint8_t type) {
  // Fill in all fields of idt[index]
  // Make sure you fill in:
  //   handler (all three parts, which requires some bit masking/shifting)
  //   type (using the parameter passed to this function)
  //   p=1 (the entry is present)
  //   dpl=0 (run the handler in kernel mode)
  //   ist=0 (we aren't using an interrupt stack table, so just pass 0)
  //   selector=IDT_CODE_SELECTOR



  // we try to split the function pointer.
  uint64_t f = (uint64_t) fn;


  uint16_t offset_0 = 0xffff & f;

  f = f >> 16;

  uint16_t offset_1 = 0xffff & f;

  f = f >> 16;

  uint32_t offset_2 = 0xffffffff & f;
  
  
  idt_entry_t* entry = &idt[index];

  entry->offset_0 = offset_0;
  entry->offset_1 = offset_1;
  entry->offset_2 = offset_2;
  entry->type = type;
  entry->present = 1;
  entry->dpl = 0;
  entry->ist = 0;
  entry->selector = IDT_CODE_SELECTOR;
}

// This struct is used to load an IDT once we've set it up
typedef struct idt_record {
  uint16_t size;
  void* base;
} __attribute__((packed)) idt_record_t;

/**
 * Initialize an interrupt descriptor table, set handlers for standard exceptions, and install
 * the IDT.
 */
void idt_setup() {
  // Step 1: Zero out the IDT, probably using memset (which you'll have to implement)
  // Write me!
  memset(idt, 0, 4096);


  // Step 2: Use idt_set_handler() to set handlers for the standard exceptions (1--21)
  // Write me!

  idt_set_handler(14, divide_error_handler, IDT_TYPE_INTERRUPT);

  kprintf("entry: %d\n", idt[0].present);
  // halt();

  // Step 3: Install the IDT
  idt_record_t record = {
    .size = sizeof(idt),
    .base = idt
  };
  __asm__("lidt %0" :: "m"(record));



}