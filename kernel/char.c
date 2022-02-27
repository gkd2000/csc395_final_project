#include "char.h"

#include "kprint.h"

char char_buffer[BUFFER_SIZE]; //> The buffer itself

/*
 * Table to convert scan codes to ascii characters.
 *
 * First 0 is because the scan codes are 1-indexed.
 * Key presses which are mapped to 0: escape, left control, left shift,
 *   right shift, left alt, caps lock
 * The zeroes near the end of the table are unmapped by the scan code set
 * 129-138 - F1 through F10
 * 139 -  NumberLock pressed
 * 140 -  ScrollLock pressed
 * 141 - 	(keypad) 7 pressed
 * 142 - 	(keypad) 8 pressed
 * 143 -  (keypad) 9 pressed
 * 144 -  (keypad) - pressed
 * 145 -  (keypad) 4 pressed
 * 146 -  (keypad) 5 pressed
 * 147 -  (keypad) 6 pressed
 * 148 -  (keypad) + pressed
 * 149 -  (keypad) 1 pressed
 * 150 -  (keypad) 2 pressed
 * 151 -  (keypad) 3 pressed
 * 152 -  (keypad) 0 pressed
 * 153 -  (keypad) . pressed
 * 154 -  f11 pressed
 * 155 -  f12 pressed
 */
uint8_t table[] = {
   0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, 9,
  'q', 'w', 'e', 'r', 't', 'y', 'u','i','o','p','[',']', '\n', 0, 'a', 's',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x','c','v',
  'b','n','m',',','.','/', 0, '*',  0, ' ', 0, 129, 130, 131, 132, 133,
  134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
  150,151,152,153,0,0,0,154,155

};

// Table to convert scan codes to uppercase characters
uint8_t uppercase_table[] = {
   0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 8, 9,
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
  'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
  'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 129, 130, 131, 132, 133,
  134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
  150, 151, 152, 153, 0, 0, 0, 154, 155
};

/**
 * Return the character corresponding to a keyboard event.
 * Side effect: if the code corresponding to backspace was received, remove an element
 * from the character buffer, so long as the character buffer is nonempty.
 * \param ch PS/2 scan code (from scan code set 1) corresponding to the keyboard event
 * \returns the character which was typed. If shift or capslock was engaged
 *          and the key pressed was one that is affected by this, then return the
 *          appropriate character. If an unprintable character was pressed, return 0.
 */
uint8_t getchar(uint8_t ch) {
  // Check if the key was...
  if (ch == 0x2A) {
    // left shift pressed
    lshift = 1;
  } else if (ch == 0x36) {
    // right shift pressed
    rshift = 1;
  } else if (ch == 0xAA) {
    // left shift released
    lshift = 0;
  } else if (ch == 0xB6) {
    // right shift released
    rshift = 0;
  } else if (ch == 0xBA) {
    // caps lock pressed
    capslock = capslock ? 0 : 1;
  } else if (ch == 0x0E) {
    // Idea: instead of calling buffer_delete() here, return the value of the backspace key (8), and handle that in
    // the handler function, where we do the other buffer operations
    // backspace pressed
    // buffer_delete();

    // put the ebackspace into the circular buffer, and deal it in sys_read
    return 8;
  }

  // A key was pressed! (as opposed to released)
  if (ch <= 0x58) {
    // Return the lowercase or uppercase version of the key pressed
    return (lshift || rshift || capslock) ? uppercase_table[ch] : table[ch];
  } else if (ch <= 0xd8 && ch >= 0x80) {
    // key was released
    // This might be unnecessary
    return 0;
  }
  return 0;
}

/**
 * Remove the most recently inserted item from the circular buffer
 * (note that ordinarily we remove things from the buffer following FIFO)
 */
void buffer_delete() {
  if (count > 0) {
    count--;
  }
}

/**
 * Insert a character into the circular buffer char_buffer
 * \param c the character to be inserted
 */
void buffer_put(char c) {
  if (count != BUFFER_SIZE) {
    char_buffer[(start + count) % BUFFER_SIZE] = c;
    count++;
  }
  return;
}

/**
 * Read one character from the keyboard buffer. If the keyboard buffer is empty this function will
 * block until a key is pressed.
 *
 * \returns the next character input from the keyboard
 */
char kgetc() {
  char c;
  // Loop until there is something in the buffer
  while(count == 0);

  // There's something in the buffer! Get it
  c = char_buffer[start];
  start = (start + 1) % BUFFER_SIZE;
  count--;
  return c;
}

/**
 * Read a line of characters from the keyboard. Read characters until the buffer fills or a newline
 * character is read. If input ends with a newline, the newline character is stored in output. The
 * string written to output is always null terminated unless the function fails for some reason.
 *
 * \param output A pointer to the beginning of an array where this function should store characters.
 *               This function will write a null terminator into the output array unless it fails.
 * \param capacity The number of characters that can safely be written to the output array
 *                 including the final null termiantor.
 * \returns The number of characters read, or zero if no characters were read due to an error.
 */
size_t kgets(char *output, size_t capacity) {
  char c = kgetc();
  size_t length = 0;
  output[length++] = c;

  while ((length < capacity-1) && (c != '\n')) {
    c = kgetc();
    // kprintf("in lower case if:%d\n", c);

    if (c == 8) {
      length = length ? length-1 : 0;
    } else {
      output[length++] = c;
    }
  }
  output[length] = '\0';
  return length;
}
