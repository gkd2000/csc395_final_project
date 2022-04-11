#pragma once

#include <stddef.h>

#include "syscall.h"

/**
 * A pared-down version of the C standard library mmap.
 * Maps pages of memory starting at the beginning of the page containing addr 
 * and continuing for at most len bytes. Pages are mapped as user-readable, writable,
 * and executable.
 * \param addr   an address on the first page to be mapped 
 * \param len    number of bytes to map
 * \param prot   unused
 * \param flags  unused
 * \param fd     unused
 * \param offset unused
 * \returns the (virtual) address of the beginning of the first page mapped
 */
void* mmap(void *addr, size_t len, int prot, int flags, int fd, size_t offset);
