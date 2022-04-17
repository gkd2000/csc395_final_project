#pragma once

#include <stddef.h>

#include "syscall.h"

/**
 * A pared-down version of the C standard library mmap.
 * Maps pages of memory starting at the beginning of the page containing addr
 * and continuing for at most len bytes. If addr is NULL, then the kernel picks
 * a place to start the mapping. Pages are mapped as user-readable; other protections
 * must be specified.
 * \param addr   an address on the first page to be mapped, or NULL if the kernel should pick
 * \param len    number of bytes to map
 * \param prot   protections for the mapped pages: all pages are marked user-readable.
 *               If prot & 0x10 = 1, then the page(s) will be writable.
 *               If prot & 0x100 = 1, then the page(s) will be executable.
 * \param flags  unused
 * \param fd     unused
 * \param offset unused
 * \returns the (virtual) address of the beginning of the first page mapped
 */
void* mmap(void *addr, size_t len, int prot, int flags, int fd, size_t offset);
