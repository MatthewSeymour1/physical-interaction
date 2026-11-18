

#include "Keyboard.h"


void setup() {
  // initialize control over the keyboard:
  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  Serial.println("==Pressing windows  & r key");
  Keyboard.press(KEY_RIGHT_GUI); // Windows key
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(1000);
  Serial.println("==Typing notepad");
  Keyboard.println("notepad");
  Keyboard.press(KEY_RETURN);
  delay(2000);
  Keyboard.releaseAll();
  //Todo: give notepad focus.
  Serial.println("==Typing Hello World");
  Keyboard.print("Hello world!");
  Keyboard.releaseAll();
  Serial.println("==finished==");
  delay(5000);
}
