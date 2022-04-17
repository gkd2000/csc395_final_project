#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mman.h>


// Round a value x up to the next multiple of y
#define ROUND_UP(x, y) ((x) % (y) == 0 ? (x) : (x) + ((y) - (x) % (y)))

// Constants for malloc
#define PAGE_SIZE 4096
// Source: https://www.ibm.com/docs/en/i/7.4?topic=ssw_ibm_i_74/apis/mmap.htm
#define PROT_READ 1
#define PROT_WRITE 2
#define MAP_SHARED 4
#define MAP_PRIVATE 2
#define MAP_FIXED 1
// Source: GNU C Library https://code.woboq.org/userspace/glibc/sysdeps/unix/sysv/linux/bits/mman-linux.h.html
#define MAP_ANONYMOUS 0x20


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

/**
 * Mimics the C standard library strpbrk
 * Locate the first occurrence in s of any character in charset, and return
 * a pointer (in s) to this character.
 * (Description from Library Functions Manual STRPBRK(3))
 *
 * \param s       the string to search through (must be null-terminated)
 * \param charset string of characters to search through (must be null-terminated)
 * \returns pointer to the first instance in s of any character in charset
 */
char* strpbrk(const char *s, const char *charset);

/**
 * Mimics the standard C function strtok
 * Separate str into tokens around the separators in sep. 
 * The first time that strtok() is called, str must be specified. If further
 * tokens are to be obtained from the same string, str should be NULL in 
 * subsequent calls to strtok. The separator string, sep, may change between 
 * calls.
 * \param str the string to separate, or NULL to continue separated the most
 *            recently passed string
 * \param sep a string of characters around which to separate str
 * \returns a pointer to the beginning of the first token found in the string, 
 *          where the delimiter character has been replaced with '\0'. If no more
 *          tokens remain, NULL is returned
 */
char * strtok(char * str, const char *restrict sep);

/**
 * Mimics the C standard library strsep
 * Locates in *stringp the first occurrence of any character specified
 * in the string delim and replaces it with '\0'. The location of the
 * next character after the delimiter is stored in *stringp.
 * If no characters in *stringp match any delimiters in the string delim,
 * then the string referenced by *stringp is unchanged and stringp* is
 * set to NULL. In either case, the original value of *stringp is returned.
 *
 * \param stringp pointer to a string to search through. *stringp must be null terminated
 * \param delim   string of delimiter characters to separate *stringp around
 * \returns the original value of *stringp
 */
char* strsep(char** stringp, const char* delim);

/**
 * Mimics the standard C function strcmp
 * Lexographically compare the null-terminated strings s1 and s2. 
 * \param s1 the first string to compare
 * \param s2 the second string to compare
 * \returns an integer, which is
 *          less than 0 if s1 is alphabetically before s2, 
 *          greater than 0 if s1 is alphabetically after s2
 *          0 if s1 adn s2 are the same
 */
int strcmp(const char *s1, const char *s2);

/**
 * Mimics the standard C function strncmp
 * Lexographically compare the first n characters (at most) of the null-terminated 
 * strings s1 and s2. Characters after '\0' are not compared.
 * \param s1 the first string to compare
 * \param s2 the second string to compare
 * \returns an integer, which is
 *          less than 0 if s1 is alphabetically before s2, 
 *          greater than 0 if s1 is alphabetically after s2
 *          0 if s1 adn s2 are the same
 */
int strncmp(const char *s1, const char *s2, size_t n);

/**
 * Mimics the C standard library strcpy
 * Copy src to dst, including the terminating '\0' character.
 * (Description from Library Functions Manual STRCPY(3))
 *
 * \param dst the location to copy src to
 * \param src the string to copy
 * \returns dst
 */
char* strcpy(char *dst, const char *src);

/**
 * Mimics the C standard library strncpy
 * Copy at most len characters from src to dst. If src is less
 * than len characters long, the rest of dst is filled with '\0' characters.
 * Otherwise, dst is not null terminated.
 * (Description from Library Functions Manual STRCPY(3))
 *
 * \param dst the location to copy src to - must be a region of at least len bytes
 * \param src the string to copy
 * \param len upper bound on number of characters to copy
 * \returns dst
 */
char* strncpy(char *dst, const char *src, size_t len);

/**
 * Mimics the standard C function atoi
 * Convert the initial portion of str to an integer representation,
 * provided that initial portion consists of numbers.
 * \param str a null-terminated string
 * \returns the integer representation of the initial portion of str, until a 
 *          non-numeral character is encountered.
 *          If str does not begin with numbers, returns 0
 */
int atoi(const char *str);

/**
 * Allocate sz bytes of memory
 * \param sz number of bytes to be allocated
 * \returns a pointer to the beginning of the allocated memory,
 *          or NULL on error
 */
void* malloc(size_t sz);
