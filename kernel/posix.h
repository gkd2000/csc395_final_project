#pragma once
#include <stdint.h>
#include <stddef.h>
#include <syscall.h>
#include <string.h>

#include "kprint.h"
#include "stivale2.h"
#include "char.h"
#include "mem.h"

#define SYS_read 0
#define SYS_write 1
#define SYS_mmap 2
#define SYS_exec 3
#define SYS_exit 4

#define BACKSPACE 8

extern void syscall_entry();

static struct stivale2_struct_tag_modules * modules_list;

/**
 * Write to size characters from buffer to fd
 * \param fd     the location to write to. Must be 1 (stdout) or 2 (stderr)
 * \param buffer string of characters to write
 * \param size   number of characters to be written
 * \return -1 on failure, number of characters written to fd on success
 */
int64_t sys_write(uint64_t fd, intptr_t buffer, uint64_t size);

/**
 * Read size characters from fd into buffer
 * \param fd     location to read from. Must be 0 (stdin)
 * \param buffer buffer to store read characters in
 * \param size   number of characters to read
 * \return number of characters read
 */
int64_t sys_read(uint64_t fd, intptr_t buffer, uint64_t size);

int64_t sys_mmap(void *addr, size_t len, int prot, int flags, int fd, size_t offset);

int64_t sys_exec(char *file, char *argv[]);

int64_t sys_exit(int status);

void module_setup(struct stivale2_struct_tag_modules *modules);
