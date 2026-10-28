
// <HID-Project.h> is a library that has gamepad functionality, which was used for the joystick. 
// It also has Keyboard functionality so I removed the initial '#include <Keyboard.h>' as having both would result in an error. 
#include <HID-Project.h>

class Button {
  public:
    int pin;
    char letter;
    int value = HIGH;
    int oldValue = HIGH;
    unsigned long lastDebounceTime = 0;
    int reading;

    Button(int p, char l) {
      pin = p;
      letter = l;
    }

    void setPinMode() {
      pinMode(pin, INPUT_PULLUP);
    }
    void getReading() {
      reading = digitalRead(pin);
    }
    void debounceButton(unsigned long debounceDelay) {
      if (reading != oldValue) {
        lastDebounceTime = millis();
      }
      if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != value) {
          value = reading;
        }
      }
      oldValue = reading;
    }
    void buttonControl() {
      if (value == LOW) {
        Keyboard.press(letter);
      }
      else {
        Keyboard.release(letter);
      }
    }
};
Button wButton(7, 'w');
Button sButton(4, 's');
Button rButton(2, 'r');
Button safetyButton(8, '\0');

bool safetyIsOn = true;
bool safetyLEDIsOn = true;
bool safetyLongPressed = false; 
unsigned long lastMeasurementTime = 0;
unsigned long safetyPressStartTime = 0;
const unsigned long debounceDelay = 50;
const unsigned long holdTime = 1000; // 1 second.

int redValue = 0;
int greenValue = 0;
const int redLEDPin = 11;
const int greenLEDPin = 10;
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
  safetyButton.setPinMode();
  wButton.setPinMode();
  sButton.setPinMode();
  rButton.setPinMode();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  Keyboard.begin();
  Gamepad.begin();
}

void loop() {

  if (millis() - lastMeasurementTime > debounceDelay && safetyIsOn == false) {
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
    lastMeasurementTime = millis();
  }

  // Button debouncers. 
  safetyButton.getReading();
  wButton.getReading();
  sButton.getReading();
  rButton.getReading();

  if (safetyButton.reading != safetyButton.oldValue) {
    safetyButton.lastDebounceTime = millis();
  }
  if ((millis() - safetyButton.lastDebounceTime) > debounceDelay) {
    if (safetyButton.reading != safetyButton.value) {
      safetyButton.value = safetyButton.reading;
      if (safetyButton.value == LOW) {
        safetyPressStartTime = millis();
        safetyLongPressed = false;
      }
      else if (!safetyLongPressed) {
        safetyIsOn = !safetyIsOn;
        Serial.println(safetyIsOn ? "safety IS on" : "safety NOT on");
      }
    }
  }
  if (safetyButton.value == LOW && safetyLongPressed == false && (millis() - safetyPressStartTime) > holdTime) {
    safetyLEDIsOn = !safetyLEDIsOn;
    safetyLongPressed = true;
    Serial.println(safetyLEDIsOn ? "safety LED IS on" : "safety LED NOT on");
  }
  safetyButton.oldValue = safetyButton.reading;
  wButton.debounceButton(debounceDelay);
  sButton.debounceButton(debounceDelay);
  rButton.debounceButton(debounceDelay);

  // Button and wheel controls
  if (safetyIsOn == false) {
    wButton.buttonControl();
    sButton.buttonControl();
    rButton.buttonControl();
    Gamepad.xAxis(mappedJoystickDistance);
    Gamepad.write();
  }
  else {
    Keyboard.releaseAll();
    Gamepad.xAxis(0);
    Gamepad.write();
  }
  // LED controls. Changing the colour of the light to red or green depending on the safetyButton state.
  // And turning off the LED entirely if it's turned off.
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
