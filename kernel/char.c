#include "char.h"

// device control 1(17) is for left control, device control 2(19) is for right control
// shift out (14) is assigned to left shift
// device control 3(18) is used for left and right alt, devicecontrol 4(20) is for caps lock
// F1 - F10 are char 129-138
// 139 - NumberLock pressed
// 140 - ScrollLock pressed
// 141 - 	(keypad) 7 pressed
// 142 - 	(keypad) 8 pressed
// 143 -  (keypad) 9 pressed
// 144 -  (keypad) - pressed
// 145 -  (keypad) 4 pressed
// 146 -  (keypad) 5 pressed
// 147 -  (keypad) 6 pressed
// 148 -  (keypad) + pressed
// 149 -  (keypad) 1 pressed
// 150 -  (keypad) 2 pressed
// 151 -  (keypad) 3 pressed
// 152 -  (keypad) 0 pressed
// 153 -  (keypad) . pressed
// 154 -  f11 pressed
// 155 -  f12 pressed

uint8_t table[] = {
  0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, 9,
  'q', 'w', 'e', 'r', 't', 'y', 'u','i','o','p','[',']', 13, 17, 'a', 's',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 14, '\\', 'z', 'x','c','v',
  'b','n','m',',','.','/', 15, '*',  18, ' ', 20, 129, 130, 131, 132, 133,
  134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
  150,151,152,153,0,0,0,154,155

};
uint8_t getchar(uint8_t ch) {
  // check whether it is a key press, or whether it is a key release.

  if (ch <= 0x58) {
    return table[ch];
  } else if (ch <= 0xd8 && ch >= 0x80) {
    return table[ch - 0x80];
  }

  return 0;
}
