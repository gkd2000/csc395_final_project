#include "posix.h"

int64_t sys_write(uint64_t fd, intptr_t buffer, size_t size)
{
  if ((fd != 1) && (fd != 2))
  {
    return -1;
  }

  int count = 0;
  char * ptr = (char *) buffer;
  for (uint64_t i = 0; i < size; i++)
  {
    kprintf("%c", ptr[i]);
    count ++;
  }
  return count;
}

int64_t sys_read(uint64_t fd, intptr_t buffer, uint64_t size) {
  if (fd)
  {
    return -1;
  }
  char c = kgetc();
  char * output = (char *) buffer;
  size_t length = 0;
  output[length++] = c;

  while ((length < size ))
  {
    c = kgetc();
    // kprintf("in lower case if:%d\n", c);

    if (c == 8)
    {
      length = length ? length - 1 : 0;
    }
    else
    {
      output[length++] = c;
    }
  }
  return length;
}
