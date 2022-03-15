#include "mman.h"

void * mmap(void *addr, size_t len, int prot, int flags, int fd, size_t offset) {
  return (void*)((intptr_t)syscall(SYS_mmap, addr, len, prot, flags, fd, offset));
}
