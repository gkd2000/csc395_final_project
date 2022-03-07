#include "elf.h"

#include "kprint.h"
#include "assembly.h"

/**
 * Run an executable stored the ELF file which begins at virtual address addr
 * \param addr the virtual address of the beginning of the ELF file
 */
void run_program(intptr_t addr) {
  // ELF files are guaranteed to start with a header; cast addr to
  // a structure matching the header exactly 
  elfhdr_t *hdr = (elfhdr_t *) addr;

  // Get the virtual address of the program entry point
  intptr_t entry = hdr->e_entry; 
  // Get the virtual address of the beginning of the program header table
  phdr_t* phead = (phdr_t *)(hdr->e_phoff + addr);
  // Get the number of entries in the program header table
  int ph_num = hdr->e_phnum;

  // Get the address of the top level page table
  uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;

  // Iterate over the program header table, mapping each segments to its place in virtual memory
  for (int i = 0; i < ph_num; i++) {
    // The current entry (segment)
    phdr_t * h = phead + i;
    kprintf("offset: %x\nvitual address: %p\n size1: %x\nsize2:%x\n", h->p_offset, h->p_vaddr, h->p_filesz, h->p_memsz);

    // If the virtual size of the current segment is nonzero, map it
    if (h->p_memsz) {
      void *file_ptr = (void *)(addr + h->p_offset); // Virtual address of the beginning of the segment
      void *v_ptr = (void *)h->p_vaddr; // Virtual address where the segment must be placed
      int executable = h->p_flags & 0x1;
      int writable = h->p_flags & 0x2;
      int readable = h->p_flags & 0x4;
      vm_map(root, v_ptr, readable, 1, executable); // Map the virtual address we need
      memcpy(v_ptr, file_ptr, h->p_memsz); // Copy the segment to its new place
    }
      kprintf("test\n");
  }
  // Jump to entry point of the function (doesn't work)
  void_function = (void_function_t) entry;
  void_function();
}
