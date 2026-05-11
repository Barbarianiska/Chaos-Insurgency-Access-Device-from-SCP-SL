// Keypad (button matrix) initialization


#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

char keys[ROWS][COLS] = {
  {' ','d',' ','r'},
  {'w','s',' ',' '},
  {' ','a',' ',' '},
  {' ',' ',' ',' '}
};

byte rowPins[ROWS] = {26, 25, 33, 32};
byte colPins[COLS] = {27, 14, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char K; // Just simple keys: W, A, S or D

/// @brief Keyboard reads input and changes current head direction
void keypad_read()
{
    K = keypad.getKey();
    if (K) 
    {
        switch(K)
        {
            case 'w': { HCD = UP; break; }
            case 'a': { HCD = LEFT; break; }
            case 's': { HCD = DOWN; break; }
            case 'd': { HCD = RIGHT; break; }
        }
    }
}