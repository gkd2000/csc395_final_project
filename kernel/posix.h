#pragma once
#include <stdint.h>
#include <stddef.h>

#include "kprint.h"


#define SYS_read 0
#define SYS_write 1

extern int64_t syscall(uint64_t nr, ...);
extern void syscall_entry();

int64_t sys_write(uint64_t fd, intptr_t buffer, uint64_t size);
