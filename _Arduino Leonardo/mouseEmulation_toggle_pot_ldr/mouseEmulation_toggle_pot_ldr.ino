/*
 * Mouse Emulation Sketch for Arduino Leonardo

 */
#include <Mouse.h>

const bool DEBUG = false;
const int potPin = A5;
const int ldrPin = A0;

const int buttonPin = 7;
int buttonState = 0;
int previousButtonState;

int toggleState = 0;

int potVal = -2;
int oldPotVal = -1;

int ldrMin = 600; //min max for ldr sensitivity
int ldrMax = 900;

//stores mapped ldrValues
int ldrNum = -1;
int lastLdr = -1;

int range = 15;
int xDistance = 0;
int yDistance = 0;
int xMultiplier = 1;
int yMultiplier = 1;


void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  
  buttonState = digitalRead(buttonPin);
  //check if there has been a change in buttonStates
  if (buttonState == 0 && previousButtonState == 1) { 
    //set the toggleState var between 1 and 0
    toggleState = 1 - toggleState;
    Serial.println();
    Serial.print(toggleState);
    Serial.println("=== Button pressed=== ");
    delay(50); //debounce delay
  }

  previousButtonState = buttonState; //store old buttonState

  if (toggleState == 1) {
    //start mouse library
    Mouse.begin();
    delay(100);

    int potValue = analogRead(potPin);
    int ldrValue = analogRead(ldrPin);

    potVal = map(potValue, 0, 1023, -10, 10);   // Map to 100 possible key states
    ldrNum = map(ldrValue, ldrMin, ldrMax, -10, 10);  //Map to 3 possible states
   
    //only update if pot value changed
    if (potVal != oldPotVal) {
      if (potVal - oldPotVal > 0) {
        Serial.println("===Right direction.");
        xMultiplier = 1;
      } else {
        Serial.println("===Left direction.");
        xMultiplier = -1;
      }
    } 

     //only update if ldr value changed
     if (ldrNum != lastLdr) {
      if (ldrNum - lastLdr > 0) {
        Serial.println("===Down direction.");
        yMultiplier = 1;
      } else {
        Serial.println("===Up direction.");
        yMultiplier = -1;
      }
    }


    if (DEBUG) {
      Serial.print(", toggleState: ");
      Serial.print(toggleState);
      Serial.print(", potVal: ");
      Serial.print(potVal);
      Serial.print(", oldPotVal: ");
      Serial.print(oldPotVal);
      Serial.print(", ldrNum: ");
      Serial.print(ldrNum);
      Serial.print(", xMultiplier: ");
      Serial.print(xMultiplier);
      Serial.print(", xDistance: ");
      Serial.print(xDistance);
      Serial.print(", yMultiplier: ");
      Serial.print(xMultiplier);
      Serial.print(", yDistance: ");
      Serial.print(xDistance);
      Serial.print(", lastLdr: ");
      Serial.println(lastLdr);
    }

    xDistance = xMultiplier * range;
    yDistance = yMultiplier * range;
    Mouse.move(xDistance, yDistance, 0);

    oldPotVal = potVal;  //store current value in oldPotVale
    lastLdr = ldrNum;    //store current ldrvalue in lastLdr

    delay(10);          // debounce and prevent flooding
  } else {
    //stop Mouse library
    Mouse.end();
  }
}
