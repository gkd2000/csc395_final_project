#pragma once

#include <stdint.h>

#define SYS_read 0
#define SYS_write 1

int syscall(uint64_t nr, ...);
