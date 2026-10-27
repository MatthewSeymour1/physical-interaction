
// <HID-Project.h> is a library that has gamepad functionality, which was used for the joystick. 
// It also has Keyboard functionality so I removed the initial '#include <Keyboard.h>' as having both would result in an error. 
#include <HID-Project.h>

bool safetyIsOn = true;
const int safetyButtonPin = 8;
const int wButtonPin = 7;
const int sButtonPin = 4;
const int rButtonPin = 2;

int safetyButtonValue = HIGH;
int oldSafetyButtonValue = HIGH;

int wButtonValue = HIGH;
int oldWButtonValue = HIGH;

int sButtonValue = HIGH;
int oldSButtonValue = HIGH;

int rButtonValue = HIGH;
int oldRButtonValue = HIGH;

unsigned long lastSafetyDebounceTime = 0;
unsigned long lastWButtonDebounceTime = 0;
unsigned long lastSButtonDebounceTime = 0;
unsigned long lastRButtonDebounceTime = 0;
unsigned long lastPrintTime = 0;
const unsigned long debounceDelay = 50;
const unsigned long holdTime = 1000; // 1 second.
unsigned long safetyPressStartTime = 0;

bool safetyLongPressed = false; 
bool safetyLEDIsOn = true;
const int redLEDPin = 11;
const int greenLEDPin = 10;
int redValue = 0;
int greenValue = 0;

const int trigPin = 5;
const int echoPin = 3;
const float maximumDistance = 50; // Default is 50.
const float minimumDistance = 3; // Default is 3.
float tenPercent = (maximumDistance - minimumDistance) / 10;
const float straightAhead = ((maximumDistance + minimumDistance) / 2);
float duration;
float distance = straightAhead;
float distanceReading = straightAhead;
float mappedJoystickDistance = 0;

float recentDistanceReadings[5] = { straightAhead, straightAhead, straightAhead, straightAhead, straightAhead };
float medianValue = straightAhead;

void setup() {

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(safetyButtonPin, INPUT_PULLUP);
  pinMode(wButtonPin, INPUT_PULLUP);
  pinMode(sButtonPin, INPUT_PULLUP);
  pinMode(rButtonPin, INPUT_PULLUP);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  Keyboard.begin();
  Gamepad.begin();
}

void loop() {

  // This is not actually a debouncer (it doesn't actually protect against bad readings), it just limits how often
  // the ultra_sonic_sensor runs / gets a reading of the distance.
  if (millis() - lastPrintTime > debounceDelay && safetyIsOn == false) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distanceReading = (duration * .0343) / 2;

    // Only accepts new readings that are within the range of maximum to minimum ± 10%.
    if (distanceReading < (maximumDistance + tenPercent) && distanceReading > minimumDistance - tenPercent) {
      distance = distanceReading;
      // Set/Enforce minimum and maximum distance.
      if (distance > maximumDistance) {
        distance = maximumDistance;
      }
      if (distance < minimumDistance) {
        distance = minimumDistance;
      }

      // Rolling the values of the recent readings back one. 
      for (int i = 0; i < 4; i++) {
        recentDistanceReadings[i] = recentDistanceReadings[i + 1];
      }
      recentDistanceReadings[4] = distance;

      // Uses median function 
      medianValue = median(recentDistanceReadings, 5);

      // Uses custom funcion to print the values of the recentDistanceReadings array.
      // printRecentReadingsArray();

      // The range of values that the joystick reads is 32767 to -32768.
      mappedJoystickDistance = map(medianValue, minimumDistance, maximumDistance, 32767, -32768);
    }
    lastPrintTime = millis();
  }

  // Controls the steering wheel when the safety is off and when the safety is on it sets the steering wheel to neutral. 
  if (safetyIsOn == false) {
    Gamepad.xAxis(mappedJoystickDistance);
    Gamepad.write();
  }
  else {
    Gamepad.xAxis(0);
    Gamepad.write();
  }

  // Button debouncers. 
  int safetyButtonReading = digitalRead(safetyButtonPin);
  int wButtonReading = digitalRead(wButtonPin);
  int sButtonReading = digitalRead(sButtonPin);
  int rButtonReading = digitalRead(rButtonPin);

  if (safetyButtonReading != oldSafetyButtonValue) {
    lastSafetyDebounceTime = millis();
  }
  if ((millis() - lastSafetyDebounceTime) > debounceDelay) {
    if (safetyButtonReading != safetyButtonValue) {
      safetyButtonValue = safetyButtonReading;
      if (safetyButtonValue == LOW) {
        safetyPressStartTime = millis();
        safetyLongPressed = false;
      }
      else if (!safetyLongPressed) {
        safetyIsOn = !safetyIsOn;
        Serial.println(safetyIsOn ? "safety IS on" : "safety NOT on");
      }
    }
  }
  if (safetyButtonValue == LOW && safetyLongPressed == false && (millis() - safetyPressStartTime) > holdTime) {
    safetyLEDIsOn = !safetyLEDIsOn;
    safetyLongPressed = true;
    Serial.println(safetyLEDIsOn ? "safety LED IS on" : "safety LED NOT on");
  }
  oldSafetyButtonValue = safetyButtonReading;

  if (wButtonReading != oldWButtonValue) {
    lastWButtonDebounceTime = millis();
  }
  if ((millis() - lastWButtonDebounceTime) > debounceDelay) {
    if (wButtonReading != wButtonValue) {
      wButtonValue = wButtonReading;
    }
  }
  oldWButtonValue = wButtonReading;

  if (sButtonReading != oldSButtonValue) {
    lastSButtonDebounceTime = millis();
  }
  if ((millis() - lastSButtonDebounceTime) > debounceDelay) {
    if (sButtonReading != sButtonValue) {
      sButtonValue = sButtonReading;
    }
  }
  oldSButtonValue = sButtonReading;

  if (rButtonReading != oldRButtonValue) {
    lastRButtonDebounceTime = millis();
  }
  if ((millis() - lastRButtonDebounceTime) > debounceDelay) {
    if (rButtonReading != rButtonValue) {
      rButtonValue = rButtonReading;
    }
  }
  oldRButtonValue = rButtonReading;

  // Button controls
  if (safetyIsOn == false) {
    if (wButtonValue == LOW) {
      Keyboard.press('w');
    } else {
      Keyboard.release('w');
    }
    if (sButtonValue == LOW) {
      Keyboard.press('s');
    } else {
      Keyboard.release('s');
    }
    if (rButtonValue == LOW) {
      Keyboard.press('r');
    } else {
      Keyboard.release('r');
    }
  }
  else {
    Keyboard.releaseAll();
  }
  // Setting the LED colour to be red or green depending on the safetyButton state. And turning off the LED entirely if it's turned off.
  if (safetyLEDIsOn) {
    if (safetyIsOn) {
      analogWrite(redLEDPin, redValue);
      analogWrite(greenLEDPin, 255);
    }
    else {
      analogWrite(redLEDPin, 255);
      analogWrite(greenLEDPin, greenValue);
    }
  }
  else {
    analogWrite(redLEDPin, 255);
    analogWrite(greenLEDPin, 255);
  }
}

//Function to get the median value of the elements in an array. 
float median(const float arr[], int size) {
  float arrayCopy[size];  // Variable-length array (compiler-dependent)

  for (int i = 0; i < size; i++) {
    arrayCopy[i] = arr[i];
  }

  // Sort the copy
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arrayCopy[j] > arrayCopy[j + 1]) {
        float t = arrayCopy[j];
        arrayCopy[j] = arrayCopy[j + 1];
        arrayCopy[j + 1] = t;
      }
    }
  }

  if (size % 2 == 1) {
    return arrayCopy[size / 2];
  }

  return (arrayCopy[size / 2 - 1] + arrayCopy[size / 2]) / 2;
}

void printRecentReadingsArray() {
  for (int i = 0; i < 5; i++) {
    Serial.print(recentDistanceReadings[i]);
  if (i < 4) {
      Serial.print(", ");
    }
    else {
      Serial.println();
    }
  }
}
