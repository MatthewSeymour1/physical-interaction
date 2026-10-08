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
 * - Reads analog value from potentiometer (range: 0-1023)
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
const bool DEBUG = true;
const int potPin = A5;
const int ldrPin = A0;
const int buttonPin = 7;

int ctrlButtonVal = -1;
int oldCtrlButtonVal = -2;

int lastKey = -2;
int keyNum = -1;

//stores mapped ldrValues
int ldrNum = -1;
int lastLdr = -2;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  int potValue = analogRead(potPin);
  int ldrValue = analogRead(ldrPin);
  keyNum = map(potValue, 0, 1023, 0, 2);  // Map to 3 possible key states
  ldrNum = map(ldrValue, 600, 900, 0, 2);  //Map to 3 possible states

  ctrlButtonVal = digitalRead(buttonPin);
  if (ctrlButtonVal == LOW && oldCtrlButtonVal == HIGH) {
      
  }

 
  if (DEBUG) {
    Serial.print("lastKey: ");
    Serial.print(lastKey);
    Serial.print(", keyNum: ");
    Serial.print(keyNum);
    Serial.print(", ldrNum: ");
    Serial.print(ldrNum);
    Serial.print(", lastLdr: ");
    Serial.println(lastLdr);
  }

  //for pot keyboard selection
  if (keyNum != lastKey) {

    switch (keyNum) {
      case 0:
        Serial.println("case 0");
        Keyboard.press('A');
        //Keyboard.release('A');
        break;
      case 1:
        Serial.println("case 1");
        Keyboard.press('B');
        // Keyboard.release('B');
        break;
      case 2:
        Serial.println("case 2");
        Keyboard.press('C');
        // Keyboard.release('B');
        break;
    }
  }

  //for ldr keyboard selection
  if (ldrNum != lastLdr) {

    switch (ldrNum) {
      case 0:
        Serial.println("case A");
        Keyboard.press('1');
        //Keyboard.release('A');
        break;
      case 1:
        Serial.println("case B");
        Keyboard.press('2');
        // Keyboard.release('B');
        break;
      case 2:
        Serial.println("case C");
        Keyboard.press('3');
        // Keyboard.release('B');
        break;
    }
  }
  lastKey = keyNum; //store current value in lastKey
  lastLdr = ldrNum; //store current ldrvalue in lastLdr21212123

  Keyboard.releaseAll();  // Release previous key
  delay(500);             // debounce and prevent flooding
}
