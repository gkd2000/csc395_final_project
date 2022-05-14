#pragma once

#include <stdint.h>

#define SYS_read 0
#define SYS_write 1
#define SYS_mmap 2
#define SYS_exec 3
#define SYS_exit 4
#define SYS_drawpixel 5
#define SYS_readmouse 6
#define SYS_update_cursor_background 7
#define SYS_gwrite 8

int syscall(uint64_t nr, ...);
