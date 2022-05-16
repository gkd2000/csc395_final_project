#include <stdint.h>
#include <stdio.h>

char* itoa_x(uint64_t value, char* arr, int num_digits);

int main() {
    char arr[20];
    char arr1[20];

    itoa_x(0xFFFFFF, arr, 6);
    itoa_x(0x00FF00, arr1, 6);

    printf("%s %s\n", arr, arr1);

    return 1;
}

char* itoa_x(uint64_t value, char* arr, int num_digits) {
  // char* arr;
  char arr_backwards[20];
  size_t i;
  for (i = 0; i < 20; i++) {
    int cval = value%16;
    if(cval >= 0 && cval <= 9) {
      arr_backwards[i] = cval + '0';
    } else {
      arr_backwards[i] = cval - 10 + 'A';
    }
    value = value/16;
    if (value == 0 && i >= num_digits - 1){
      break;  
    }
  }

  for(int j = 0; j <= i; j++) {
    printf("%c", arr_backwards[j]);
  }

  printf("\n");

  for(int j = 0; j <= i; j++) {
    arr[j] = arr_backwards[i - j];
  }
  arr[i+1] = '\0';

  return arr;
}