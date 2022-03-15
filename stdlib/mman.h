#include <stddef.h>

#include "syscall.h"

void * mmap(void *addr, size_t len, int prot, int flags, int fd, size_t offset);
