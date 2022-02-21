#include "mem.h"

// The freelist. Should this be in the .h and static?
node_t* freelist = NULL;

/**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 * \param arr  pointer to the memory to be written to
 * \param c    value to be written
 * \param size number of bytes to be written
 * \return arr (the first argument)
 */
void* memset(void *arr, int c, uint32_t size) {

  // Walk through each byte of arr, setting the value to c
  uint8_t *temp = arr;
  for (int i = 0; i < size; i++) {
    temp[i] = c;
  }
  return arr;
}

/**
 * Break up a large piece of memory and add it to the free list
 * \param base   the physical start of the usable memory
 * \param length length of the usable memory in bytes
 */
void add_memory(uint64_t base, uint64_t length) {
  // Round the starting address up to the nearest multiple of a page
  if (base % PAGE_SIZE) {
    length -= (PAGE_SIZE - (base % PAGE_SIZE));
    base += (PAGE_SIZE - (base % PAGE_SIZE));
  }

  // Iterate through the memory given, adding it to the free list in chunks of
  // PAGE_SIZE bytes
  node_t* prev = freelist;
  for (int i = 0; i < length/PAGE_SIZE; i++) {
    node_t * new_node = (node_t *)(uintptr_t)(base + PAGE_SIZE*i);
    new_node->serial_number = SERIAL_NUMBER;
    new_node->next = prev;
    prev = new_node;
  }
  freelist = prev;
}

/**
 * Does not currently print the usable memory, as reported by the bootloader.
 * Enables write protection and creates the freelist
 * \param virtual  the stivale2 struct tag which gives the beginning of the HHDM
 * \param physical the stivale2 memory map struct tag, which reports the memory map
 *                 the memory map built by the bootloader
 */
void get_usable_memory(struct stivale2_struct_tag_hhdm *virtual, struct stivale2_struct_tag_memmap *physical) {
  // Store the start of the HHDM
  virtual_offset = virtual->addr;

  // Enable write protection
  uint64_t cr0 = read_cr0();
  cr0 |= 0x10000;
  write_cr0(cr0);

  // Loop through physical memory entries, looking for usable memory
  for (int i = 0; i < physical->entries; i++) {
    if (physical->memmap[i].type == 1) {

      // Add usable memory to the freelist
      add_memory(physical->memmap[i].base, physical->memmap[i].length);

      // We found usable memory! Print its physical and virtual location
      // kprintf("0x%x-0x%x mapped at 0x%x-0x%x\n", physical->memmap[i].base,
      //         physical->memmap[i].base + physical->memmap[i].length, physical->memmap[i].base + virtual_offset,
      //         physical->memmap[i].base + physical->memmap[i].length + virtual_offset);
    }
  }

  kprintf("first in the free list:%p\n", freelist);
}

/**
 * Check the permissions of a page table entry
 * \param table the page table entry
 * \returns 1 if the entry is not present
 *          0 if the entry is present
 */
uint8_t check_permission(page_table_t * table) {

  if (!table->present) {
    kprintf("        not present\n");
    return 1;
  }
  kprintf("        ");
  if (table->kernel) {
    kprintf("user ");
  } else {
    kprintf("kernel ");
  }

  if (table->writable) {
    kprintf("writable ");
  }

  if (!table->no_execute) {
    kprintf("executable");
  }

  kprintf(" -> %p\n", table->physical_addr << 12);

  return 0;
};

/**
 * Translate a virtual address to its mapped physical address
 *
 * \param address     The virtual address to translate
 */
void translate(uintptr_t page_table, void *address) {
  // split the address into 4 level page table

  uintptr_t temp2 = (uintptr_t)address;
  uint16_t offset = temp2 & 0x7ff;
  uint16_t level1 = (temp2 >> 12) & 0x1ff;
  uint16_t level2 = (temp2 >> 21) & 0x1ff;
  uint16_t level3 = (temp2 >> 30) & 0x1ff;
  uint16_t level4 = (temp2 >> 39) & 0x1ff;

  kprintf("Translating: %p\n", address);

  page_table_t *level4_table = ((page_table_t *)(page_table + virtual_offset))+ level4;
  kprintf("    Level 4 (index %d of %p)\n", level4, page_table);
  if(check_permission(level4_table)) {
    return;
  }


  uintptr_t level3_start_of_the_page = (level4_table->physical_addr) << 12;
  page_table_t *level3_table = ((page_table_t *) (level3_start_of_the_page + virtual_offset)) + level3;
  kprintf("    Level 3 (index %d of %p)\n", level3, level3_start_of_the_page);
  if (check_permission(level3_table)) {
    return;
  }

  uintptr_t level2_start_of_the_page = (level3_table->physical_addr) << 12;
  page_table_t *level2_table = ((page_table_t *)(level2_start_of_the_page + virtual_offset)) + level2;
  kprintf("    Level 2 (index %d of 0x%x)\n", level2, level2_start_of_the_page);
  if (check_permission(level2_table)) {
    return;
  }

  uintptr_t level1_start_of_the_page = level2_table->physical_addr << 12;
  page_table_t *level1_table = ((page_table_t *) (level1_start_of_the_page + virtual_offset)) + level1;
  kprintf("    Level 1 (index %d of %p)\n", level1, level1_start_of_the_page);
  if (check_permission(level1_table)) {
    return;
  }

  // uint64_t temp = (level1_table[level1]).physical_addr + addr->offset;
  uint64_t physical_addr = (level1_table->physical_addr << 12) + offset;


  kprintf("%p maps to 0x%x\n", address, physical_addr);
}


/**
 * Allocate a page of physical memory.
 * \returns the physical address of the allocated physical memory or 0 on error.
 */
uintptr_t pmem_alloc() {
  node_t * temp = freelist;
  if (temp) {
    freelist = temp->next;
    temp->serial_number = 0;
    return (uintptr_t) temp;
  }
  return 0;
}

/**
 * Free a page of physical memory.
 * \param p is the physical address of the page to free, which must be page-aligned.
 */
void pmem_free(uintptr_t p) {
  node_t * temp = (node_t *)p;
  if (temp->serial_number != SERIAL_NUMBER) {
    temp->serial_number = SERIAL_NUMBER;
    temp->next = freelist;
    freelist = temp;
  }
}

/**
 * Add an entry to a page table, and allocate the corresponding page
 * \param table      The page table entry to populate
 * \param user       Should the page be user-accessible?
 * \param writable   Should the page be writable?
 * \param executable Should the page be executable?
 * \returns true if no page was allocated, false otherwise 
 */
bool malloc_page(page_table_t *table, bool user, bool writable, bool executable) {
  
  // Get a page for the new entry 
  uintptr_t new_page = pmem_alloc();
  if (new_page == 0) {
    // pmem_alloc failed
    return 1;
  }
  // Mark the page as present and fill in the other relevant fields
  table->present = 1;
  table->physical_addr = new_page >> 12;
  table->kernel = user;
  table->writable = writable;
  table->no_execute = !executable;

  // Fill in the new page with zeroes
  new_page += virtual_offset;
  memset((void*)new_page, 0, PAGE_SIZE);

  return 0;
}

/**
 * Map a single page of memory into a virtual address space.
 * \param root       The physical address of the top-level page table structure
 * \param address    The virtual address to map into the address space, must be page-aligned
 * \param user       Should the page be user-accessible?
 * \param writable   Should the page be writable?
 * \param executable Should the page be executable?
 * \returns true if the mapping succeeded, or false if there was an error
 */
bool vm_map(uintptr_t root, uintptr_t address, bool user, bool writable, bool executable) {

  // Break the virtual address into the page table addresses
  uintptr_t temp2 = (uintptr_t)address;
  uint16_t level1 = (temp2 >> 12) & 0x1ff;
  uint16_t level2 = (temp2 >> 21) & 0x1ff;
  uint16_t level3 = (temp2 >> 30) & 0x1ff;
  uint16_t level4 = (temp2 >> 39) & 0x1ff;

  // Traverse the page tables, mapping new pages as necessary 
  page_table_t *level4_table = ((page_table_t *)(root + virtual_offset)) + level4;
  if (!level4_table->present) {
    if(malloc_page(level4_table, 1, 1, 0)) {
      return false;
    }
  }

  uintptr_t level3_start_of_the_page = (level4_table->physical_addr) << 12;
  page_table_t *level3_table = ((page_table_t *)(level3_start_of_the_page + virtual_offset)) + level3;
  if (!level3_table->present) {
    if(malloc_page(level3_table, 1, 1, 0)) {
      return false;
    }
  }

  uintptr_t level2_start_of_the_page = (level3_table->physical_addr) << 12;
  page_table_t *level2_table = ((page_table_t *)(level2_start_of_the_page + virtual_offset)) + level2;
  if (!level2_table->present) {
    if(malloc_page(level2_table, 1, 1, 0)) {
      return false;
    }
  }

  uintptr_t level1_start_of_the_page = level2_table->physical_addr << 12;
  page_table_t *level1_table = ((page_table_t *)(level1_start_of_the_page + virtual_offset)) + level1;
  if (!level1_table->present) {
    if(malloc_page(level1_table, user, writable, executable)) {
      return false;
    }
  } else {
    // There was already a page mapped at that address
    return false;
  }

  // Page mapped successfully!
  return true;
}
