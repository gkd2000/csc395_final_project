#pragma once

#include <stdint.h>
#include <stddef.h>

#include "syscall.h"

#define SYS_read  0
#define SYS_write 1

/**
 * Mimics functionality of C standard library write function.
 * Write a specified number of bytes from a buffer to a given file descriptor.
 * \param filedes the file to write to.
 *    Note that right now, filedes must be 1 or 2 (stdout or stderr, respectively),
 *    but that check happens in the system call itself
 * \param buf     array of values to write to the location specified by filedes
 * \param nbyte   number of bytes to write
 * \returns the number of bytes written, or -1 on error
 */
size_t write(int filedes, const void *buf, size_t nbyte);

/**
 * Mimics functionality of C standard library getline function.
 * Read from the location specified by filedes until a newline character
 * is encountered or a given number of bytes are read. 
 * Note that the C standard library function getline allows for dynamic 
 * expansion of the buffer *linep, as well as corresponding updates to 
 * *linecapp (the size of the buffer). This implementation simply reads 
 * a specified number of bytes. It is up to the caller to ensure that 
 * *linep is large enough to hold *linecapp bytes.
 * \param linep    pointer to an array to store the values read
 * \param linecapp pointer to the maximum number of bytes to read
 * \param filedes  the file to read from
 *    Note that right now, filedes must be 0 (stdin), but that check happens 
 *    in the system call itself
 * \returns the number of bytes read and stored in *linep on success
 *          -1 on error
 */
size_t getline(char** linep, size_t* linecapp, int filedes);
