#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 * \param arr  pointer to the memory to be written to
 * \param c    value to be written
 * \param size number of bytes to be written
 * \returns arr (the first argument)
 */
void *memset(void *arr, int c, uint32_t size);

/**
 * Mimics the standard C function memcpy
 * Copies size bytes of data from des to src.
 * The locations indicated by des and src should not overlap
 * \param des  pointer to the memory location to copy data to
 * \param src  pointer to the memory location to copy data from
 * \param size number of bytes to copy
 * \returns pointer to the first byte of copied memory
 */
void *memcpy(void *des, void *src, size_t size);

/**
 * Counts the number of characters, excluding the null terminator, in a string
 * \param str the string to count characters of
 * \return the number of cahracters in str, excluding the null terminator
 */
uint32_t strlen(const char *str);

char * strpbrk(const char *s, const char *charset);

char * strtok(char *restrict str, const char *restrict sep);

char * strtok_r(char *restrict str, const char *restrict sep,
         char **restrict lasts);

char * strsep(char **stringp, const char *delim);

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);

char * stpcpy(char *dst, const char *src);

char * stpncpy(char *dst, const char *src, size_t len);

int atoi(const char *str);
