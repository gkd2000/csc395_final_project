#include "string.h"

char *strtok_s = NULL;
/**
 * Mimics the standard C function memset.
 * Writes size bytes of value c to the memory pointed to by arr.
 * \param arr  pointer to the memory to be written to
 * \param c    value to be written
 * \param size number of bytes to be written
 * \returns arr (the first argument)
 */
void *memset(void *arr, int c, uint32_t size)
{

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
void *memcpy(void *des, void *src, size_t size)
{
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
uint32_t strlen(const char *str)
{
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


bool contain(char c, char* l) {
  while(l) {
    if (c == *l++) {
      return true;
    }
  }
  return false;
}
char *strtok(char * str, const char *restrict sep) {
  char * res = str ? str : strtok_s;
  char * temp = res;
  while (*temp != '\0' && contain(*temp, sep));
  if (*temp == '\0') {
    strtok_s = NULL;
  } else {
    strtok_s = temp+1;
    *temp = '\0';
  }

  return res;
}


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

int strcmp(const char *s1, const char *s2) {
  char * p = s1;
  char * q = s2;
  while ((*p && *q) && (*p++ == *q++));

  return *p - *q;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  for (int i = 0; i < n; i++) {
    if (s1[i]!=s2[i]) {
      return s1[i] - s2[i];
    }
  }
  return 0;
}
