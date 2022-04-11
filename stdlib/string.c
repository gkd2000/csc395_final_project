#include "string.h"

// Global for strtok to save progress between calls
char *strtok_s = NULL;

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
 * \param str the string to count characters of. Must be null-terminated
 * \return the number of characters in str, excluding the null terminator
 */
uint32_t strlen(const char *str) {
  uint32_t length = 0;
  const char *temp = str;

  // Iterate through str, incrementing our counter
  while (*temp != '\0') {
    length++;
    temp++;
  }

  return length;
}

// Ask Paul
/**
 * Determines whether a string l contains a character c
 * \param c the character to search for
 * \param l the string to search through
 * \returns true if c appears in l, and false otherwise
 */
bool contain(char c, const char *restrict l) {
  while(l) {
    if (c == *l++) {
      return true;
    }
  }
  return false;
}

// Ask Paul
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
char *strtok(char * str, const char *restrict sep) {
  // If str is NULL, use the global strtok_s (from a previous call)
  char * res = str ? str : strtok_s;
  char * temp = res;
  while (*temp != '\0' && contain(*temp, sep)); // How does this ever end? Is temp being incremented somewhere?
  if (*temp == '\0') {
    strtok_s = NULL;
  } else {
    strtok_s = temp+1;
    *temp = '\0';
  }

  return res;
}

/**
 * Mimics the standard C function atoi
 * Convert the initial portion of str to an integer representation,
 * provided that initial portion consists of numbers.
 * \param str a null-terminated string
 * \returns the integer representation of the initial portion of str, until a 
 *          non-numeral character is encountered.
 *          If str does not begin with numbers, returns 0
 */
int atoi(const char* str) {
  int res = 0;
  for (int i = 0; i < strlen(str); i++) {
    if (str[i]>='0' && str[i] <= '9') {
      res = res*10+str[i]-'0';
    } else {
      return res;
    }
  }
  return res;
}

/**
 * Mimics the standard C function atoi
 * Lexographically compare the null-terminated strings s1 and s2. 
 * \param s1 the first string to compare
 * \param s2 the second string to compare
 * \returns an integer, which is
 *          less than 0 if s1 is alphabetically before s2, 
 *          greater than 0 if s1 is alphabetically after s2
 *          0 if s1 adn s2 are the same
 */
int strcmp(const char *s1, const char *s2) {
  while ((*s1 && *s2) && (*s1++ == *s2++));

  return *s1 - *s2;
}

// Ask Paul: check for null terminator?
/**
 * Mimics the standard C function atoi
 * Lexographically compare the first n characters (at most) of the null-terminated 
 * strings s1 and s2. Characters after '\0' are not compared.
 * \param s1 the first string to compare
 * \param s2 the second string to compare
 * \returns an integer, which is
 *          less than 0 if s1 is alphabetically before s2, 
 *          greater than 0 if s1 is alphabetically after s2
 *          0 if s1 adn s2 are the same
 */
int strncmp(const char *s1, const char *s2, size_t n) {
  for (int i = 0; i < n; i++) {
    if (s1[i]!=s2[i]) {
      return s1[i] - s2[i];
    }
  }
  return 0;
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


// Globals for malloc
void* bump = NULL;
size_t space_remaining = 0;

/**
 * Allocate sz bytes of memory
 * \param sz number of bytes to be allocated
 * \returns a pointer to the beginning of the allocated memory,
 *          or NULL on error
*/
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
}

// Our malloc implementation does not support freeing
void free(void* p) {
  // Do nothing
}
