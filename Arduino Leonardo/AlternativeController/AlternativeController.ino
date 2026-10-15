// Currently this contains just the code from the ultra_sonic_sensor file. and it just prints to the serial console thingy the distance.
// Though first, try and get two simple buttons to emulate the keyboard for 'W' and 'S'. Make sure to have a safety. A button that you need to hold down for the keyboard emulation to work. To avoid arduino taking over your pc D:

// To deal with the ultra_sonic_sensor's occasional (incorrect) very high reading, perhaps using a rolling median filter. store the 3 most recent readings, and take the median reading.
// So for example, 40, 42, 1000. it would choose 42. every time a new reading is received, add that to the end of the array and remove the first reading in the array. 
// So if the 3 most recent readings are 40, 42, 1000 and the next reading is 44, then the array would become 42, 1000, 44. and the median value of that would be 44.
// would need to store at least 4 or 5 recent readings. when there is noise, the bad readings can have 2 or even 3 bad readings in a row. 

#include <Keyboard.h>

bool safetyIsOn = true;
const int safetyButtonPin = 8;
const int wButtonPin = 7;
const int sButtonPin = 4;

int safetyButtonValue = HIGH;
int oldSafetyButtonValue = HIGH;

int wButtonValue = HIGH;
int oldWButtonValue = HIGH;

int sButtonValue = HIGH;
int oldSButtonValue = HIGH;

unsigned long lastSafetyDebounceTime = 0;
unsigned long lastWButtonDebounceTime = 0;
unsigned long lastSButtonDebounceTime = 0;
unsigned long lastPrintTime = 0;
unsigned long debounceDelay = 50;

const int redLEDPin = 11;
const int greenLEDPin = 10;

int redValue = 0;
int greenValue = 0;



// This is the ultra_sonic_sensor variables.
const int trigPin = 5;
const int echoPin = 3;
const float minimumDistance = 3;
const float maximumDistance = 30;
const float straightAhead = ((maximumDistance - minimumDistance) / 2); // 13.5
const float deadZone = 5;
float duration;
float distance = straightAhead;
// end of the ultra_sonic_sensor variables.

void setup() {
  // This is the ultra_sonic_sensor code in SETUP.
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // end of the ultra_sonic_sensor code in SETUP.

  Serial.begin(9600);
  pinMode(safetyButtonPin, INPUT_PULLUP);
  pinMode(wButtonPin, INPUT_PULLUP);
  pinMode(sButtonPin, INPUT_PULLUP);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  Keyboard.begin();
  Serial.println(safetyIsOn ? "safety IS on" : "safety NOT on");
}

void loop() {

  // This is the ultra_sonic_sensor code in LOOP.

  if (millis() - lastPrintTime > debounceDelay && safetyIsOn == false) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;

    // Set minimum and maximum distance. 
    if (distance > 30) {
      distance = 30;
    }
    if (distance < 3) {
      distance = 3;
    }

    Serial.print("Distance: ");
    Serial.println(distance);

    lastPrintTime = millis();
  }

  if (safetyIsOn == false && distance < straightAhead - deadZone) {
    Keyboard.press('d');
  }
  else {
    Keyboard.release('d');
  }

  if (safetyIsOn == false && distance > straightAhead + deadZone) {
    Keyboard.press('a');
  }
  else {
    Keyboard.release('a');
  }

  // end of ultra_sonic_sensor code in LOOP.





  int safetyButtonReading = digitalRead(safetyButtonPin);
  int wButtonReading = digitalRead(wButtonPin);
  int sButtonReading = digitalRead(sButtonPin);

  if (safetyButtonReading != oldSafetyButtonValue) {
    lastSafetyDebounceTime = millis();
  }
  if ((millis() - lastSafetyDebounceTime) > debounceDelay) {
    if (safetyButtonReading != safetyButtonValue) {
      safetyButtonValue = safetyButtonReading;
      if (safetyButtonValue == LOW) {
        safetyIsOn = !safetyIsOn;
        Serial.println(safetyIsOn ? "safety IS on" : "safety NOT on");
      }
    }
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

  if (safetyIsOn == false && wButtonValue == LOW) {
    Keyboard.press('w');
  }
  else {
    Keyboard.release('w');
  }

  if (sButtonReading != oldSButtonValue) {
    lastSButtonDebounceTime = millis();
  }
  if ((millis() - lastSButtonDebounceTime) > debounceDelay) {
    if (sButtonReading != sButtonValue) {
      sButtonValue = sButtonReading;
    }
  }
  oldSButtonValue = sButtonReading;

  if (safetyIsOn == false && sButtonValue == LOW) {
    Keyboard.press('s');
  }
  else {
    Keyboard.release('s');
  }



  if (safetyIsOn == true) {
    analogWrite(redLEDPin, redValue);
    analogWrite(greenLEDPin, 255);
  }
  else {
    analogWrite(redLEDPin, 255);
    analogWrite(greenLEDPin, greenValue);
  }

  
}












