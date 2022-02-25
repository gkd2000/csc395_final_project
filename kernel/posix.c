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
