#include "elf.h"

#include "kprint.h"
#include "assembly.h"

void run_program(intptr_t addr) {
  elfhdr_t *hdr = (elfhdr_t *) addr;

  intptr_t entry = hdr->e_entry;
  phdr_t * phead = (phdr_t *)(hdr->e_phoff + addr);
  int num = hdr->e_phnum;

  uintptr_t root = read_cr3() & 0xFFFFFFFFFFFFF000;
  for (int i = 0; i < num; i++) {
    phdr_t * h = phead + i;
    kprintf("offset: %x\nvitual address: %p\n size1: %x\nsize2:%x\n", h->p_offset, h->p_vaddr, h->p_filesz, h->p_memsz);

    if (h->p_memsz) {
      void *file_ptr = (void *)(addr + h->p_offset);
      void *v_ptr = (void *)h->p_vaddr;
      int executable = h->p_flags & 0x1;
      int writable = h->p_flags & 0x2;
      int readable = h->p_flags & 0x4;
      vm_map(root, v_ptr, readable, 1, executable);
      memcpy(v_ptr, file_ptr, h->p_memsz);
    }
      kprintf("test\n");
  }
  void_function = (void_function_t) entry;
  void_function();
}
