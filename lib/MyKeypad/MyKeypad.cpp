#include "./MyKeypad.h"
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'1', '4', '7', '*'},
    {'2', '5', '8', '0'},
    {'3', '6', '9', '#'},
    {'A', 'B', 'C', 'D'}};
byte rowPins[ROWS] = {27, 14, 12, 13};
byte colPins[COLS] = {32, 33, 25, 26};

Keypad setupKeypad()
{
    Serial.println("Setting up keyboard");
    return Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
}