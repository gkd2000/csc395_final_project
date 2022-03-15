#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
// #include <stdio.h>

#include "string.h"
#include "syscall.h"

#define STDOUT 1

size_t read(int fildes, void *buf, size_t nbyte);
