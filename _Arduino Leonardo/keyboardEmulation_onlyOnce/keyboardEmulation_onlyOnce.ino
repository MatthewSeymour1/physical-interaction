/*
 * Keyboard Emulation Sketch for Arduino Leonardo
 *
 * This sketch reads an analog input from a potentiometer connected to pin A5
 * and emulates keyboard presses based on the potentiometer position.
 *
 * Description:
 * This sketch continuously reads the analog value from a potentiometer
 * connected to pin A5. The value is mapped to one of three key states (0, 1, or 2).
 * When the potentiometer position changes, the sketch sends a keyboard press
 * event for the corresponding key ('A', 'B', or 'C'). The keys are pressed
 * and immediately released using Keyboard.releaseAll().
 *
 * Hardware Setup:
 * - Potentiometer connected to analog pin A5
 * - Arduino Leonardo (required for Keyboard functionality)
 *
 * Functionality:
 * - Reads analog value from potentiometer (range: 0-1023)notepad


 * - Maps the value to 3 possible key states (0, 1, 2)
 * - Sends keyboard press events based on the mapped value:
 *   - Pot position range 0-341: Sends 'A' key press
 *   - Pot position range 342-682: Sends 'B' key press
 *   - Pot position range 683-1023: Sends 'C' key press
 * - Includes serial output for debugging (lastKey and keyNum values)
 * - Implements debouncing with a 500ms delay to prevent key flooding
 * - Uses Keyboard.releaseAll() to release keys after each press
 *
 * Note: The Leonardo board is required for keyboard emulation functionality.
 */

#include <Keyboard.h>
const int potPin = A5;
int lastKey = -2;
int keyNum = -1;

void setup() {
  Serial.begin(9600);
 
}

void loop() {
  int potValue = analogRead(potPin);
  keyNum = map(potValue, 0, 1023, 0, 3); // Map to 3 possible key states
  
  Serial.print("lastKey: ");
  Serial.print(lastKey);
  Serial.print(", keyNum: ");
  Serial.println(keyNum);

  if (keyNum != lastKey) {
    
    switch (keyNum) {
      case 0:
        Serial.println("case 0");
        Keyboard.press('W');
        delay(100);
        Keyboard.release('W');
        break;
      case 1:
        Serial.println("case 1");
        Keyboard.press('A');
        delay(100);
        Keyboard.release('A');
        break;
      case 2:
        Serial.println("case 2");
        Keyboard.press('S');
        delay(100);
        Keyboard.release('S');
        break;
      case 3:
        Serial.println("case 3");
        Keyboard.press('D');
        delay(100);
        Keyboard.release('D');
        break;
    }
    
  }
  lastKey = keyNum;
  Keyboard.releaseAll(); // Release previous key
  delay(500); // debounce and prevent flooding
}

