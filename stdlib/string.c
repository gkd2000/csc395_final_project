#include "string.h"

/**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 * \param arr  pointer to the memory to be written to
 * \param c    value to be written
 * \param size number of bytes to be written
 * \returns arr (the first argument)
 */
void *memset(void *arr, int c, uint32_t size) {

  // Walk through each byte of arr, setting the value to c
  uint8_t *temp = arr;
  for (int i = 0; i < size; i++)
  {
    temp[i] = c;
  }
  return arr;
}

/**
 * Mimics the standard C function memcpy
 * Copies size bytes of data from des to src.
 * The locations indicated by des and src should not overlap
 * \param des  pointer to the memory location to copy data to
 * \param src  pointer to the memory location to copy data from
 * \param size number of bytes to copy
 * \returns pointer to the first byte of copied memory
 */
void *memcpy(void *des, void *src, size_t size) {
  uint8_t *des1 = (uint8_t *)des;
  uint8_t *src1 = (uint8_t *)src;
  for (size_t i = 0; i < size; i++)
  {
    des1[i] = src1[i];
  }
  return des;
}

/**
 * Counts the number of characters, excluding the null terminator, in a string
 * \param str the string to count characters of
 * \return the number of cahracters in str, excluding the null terminator
 */
uint32_t strlen(const char *str) {
  uint32_t length = 0;
  const char *temp = str;

  // Iterate through str, incrementing our counter
  while (*temp != '\0')
  {
    length++;
    temp++;
  }

  return length;
}

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
char* strsep(char** stringp, const char* delim) {
  if(*stringp == NULL) {
    return NULL;
  }

  char* orig_str = *stringp;
  int delim_idx;
  int str_idx = 0;

  // Loop over the given string
  while(orig_str[str_idx] != '\0') {
    // Check the current character in the string given against each delimiter
    while(delim[delim_idx] != '\0') {
      if(delim[delim_idx] == orig_str[str_idx]) {
        // We have a match! Set the delimiter to the null terminator and 
        // store the location of the next character in *stringp
        (*stringp)[str_idx] = '\0';
        *stringp = (*stringp) + str_idx + 1;
        return orig_str;
      } else {
        // No match. Check the next delimiter
        delim_idx++;
      }
    }
    delim_idx = 0;
    str_idx++;
  }

  // We reached the end of the string, having found no matches for any delimiters.
  *stringp = NULL;
  return orig_str;
}

/**
 * Mimics the C standard library strcpy
 * Copy src to dst, including the terminating '\0' character.
 * (Description from Library Functions Manual STRCPY(3)) 
 * 
 * \param dst the location to copy src to
 * \param src the string to copy
 * \returns dst
 */
char* strcpy(char *dst, const char *src) {
  int i = 0;
  while(src[i] != '\0') {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';
  return dst;
}

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
char* strncpy(char *dst, const char *src, size_t len) {
  int found_end = 0;
  for(size_t i = 0; i < len; i++) {
    if(!found_end) {
      dst[i] = src[i];
      if(src[i] == '\0') {
        found_end = 1;
      }
    } else {
      dst[i] = '\0';
    }
  }
  return dst;
}

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
char* strpbrk(const char *s, const char *charset) {
  int str_idx = 0;
  int charset_idx = 0;
  while(s[str_idx] != '\0') {
    // Check the current character in the string given against each delimiter
    while(charset[charset_idx] != '\0') {
      if(charset[charset_idx] == s[str_idx]) {
        // We have a match! Return a pointer to the charater in s
        return (char*) &(s[str_idx]);
      } else {
        // No match. Check the next delimiter
        charset_idx++;
      }
    }
    charset_idx = 0;
    str_idx++;
  }
  return NULL;
}

/**
 * Mimics C standard library strcmp
 */
int strcmp(const char *s1, const char *s2) {
  int i = 0;
  while(s1[i] == s2[i]) {
    if(s1[i] == '\0') {
      // We reached the end of both strings
      return 0;
    } else {
      i++;
    }
  }
  if(s1[i] == '\0' || s1[i] - s2[i] < 0) {
    return -1;
  } else {
    return 1;
  }
}

// Globals for malloc
void* bump = NULL;
size_t space_remaining = 0;

/**
 * Allocate sz bytes of memory
 * NO ONE USE THIS YET, it depends on mmap system call
 * \param sz number of bytes to be allocated
 * \returns a pointer to the beginning of the allocated memory,
 *          or NULL on error
 
void* malloc(size_t sz) {
  // Round sz up to a multiple of 16
  sz = ROUND_UP(sz, 16);

  // Do we have enough space to satisfy this allocation?
  if (space_remaining < sz) {
    // No. Get some more space using `mmap`
    size_t rounded_up = ROUND_UP(sz, PAGE_SIZE);
    void* newmem = mmap(NULL, rounded_up, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    // Check for errors
    if (newmem == NULL) {
      return NULL;
    }

    bump = newmem;
    space_remaining = rounded_up;
  }

  // Grab bytes from the beginning of our bump pointer region
  void* result = bump;
  bump += sz;
  space_remaining -= sz;

  return result;
}*/

// Our malloc implementation does not support freeing right now
void free(void* p) {
  // Do nothing
}
