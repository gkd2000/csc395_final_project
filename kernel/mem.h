#pragma once
#include <stdint.h>
#include <stddef.h>

// memset takes in any pointer and the size of the memory that the pointer points to.
// it will set all the memories to 0, so it does not have random data.
// we assume that the pointer is a multiple 1 byte.
// it set all the entries to c, which is a parameter
void* memset(void *arr, int c, uint32_t size);
