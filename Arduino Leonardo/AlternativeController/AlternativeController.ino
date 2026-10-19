// To deal with the ultra_sonic_sensor's occasional (incorrect) very high reading, perhaps using a rolling median filter. store the 3 most recent readings, and take the median reading.
// So for example, 40, 42, 1000. it would choose 42. every time a new reading is received, add that to the end of the array and remove the first reading in the array. 
// So if the 3 most recent readings are 40, 42, 1000 and the next reading is 44, then the array would become 42, 1000, 44. and the median value of that would be 44.
// would need to store at least 4 or 5 recent readings. when there is noise, the bad readings can have 2 or even 3 bad readings in a row. 

// The code that constrains the distance readings to a minimum and maximum distance can be improved. 
// Right now it sets any reading above the maxium to equal the maximum. But that makes it so if you take your hand away 
// from the ultra_sonic_sensor then it will by default read the max distance, forcing a speedZone = 4 left turn. 
// Ideally, if you take your hand away, it should default to aiming mappedStraightAhead. 
// but if you are purposefully trying to do a speedZone = 4 left turn, and you accidentally move your hand above 
// the maximum distance value, it should not aim at mappedStraightAhead, it should aim at speedZone = 4 left turn. 
// so in some cases set the distance to be straight ahead. and in some cases set the distance to be the maximum. 

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

unsigned long lastVerySlightlyPressedTimeD = 0;
unsigned long lastSlightlyPressedTimeD = 0;
unsigned long lastPressedTimeD = 0;
unsigned long pressedDelayD = 0;
unsigned long slightlyPressedDelayD = 0;

unsigned long lastVerySlightlyPressedTimeA = 0;
unsigned long lastSlightlyPressedTimeA = 0;
unsigned long lastPressedTimeA = 0;
unsigned long pressedDelayA = 0;
unsigned long slightlyPressedDelayA = 0;

unsigned long debounceDelay = 50;
int speedZone = 0;

const int redLEDPin = 11;
const int greenLEDPin = 10;

int redValue = 0;
int greenValue = 0;

bool wasVerySlightlyPressedD = false;
bool wasSlightlyPressedD = false;
bool wasPressedD = false;

bool wasVerySlightlyPressedA = false;
bool wasSlightlyPressedA = false;
bool wasPressedA = false;

// This is the ultra_sonic_sensor variables.
const int trigPin = 5;
const int echoPin = 3;
const float maximumDistance = 50;
const float minimumDistance = 3;
const float straightAhead = ((maximumDistance + minimumDistance) / 2);
const float mappedStraightAhead = 50;
const float mappedDeadZone = 10;
float duration;
float distance = straightAhead;
float mappedDistance = mappedStraightAhead;
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

  // This is not actually a debouncer (it doesn't actually protect against bad readings), it just limits how often
  // the ultra_sonic_sensor runs / gets a reading of the distance. 
  if (millis() - lastPrintTime > debounceDelay && safetyIsOn == false) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    Serial.println(speedZone);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;

    // Set/Enforce minimum and maximum distance. 
    if (distance > maximumDistance) {
      distance = maximumDistance;
    }
    if (distance < minimumDistance) {
      distance = minimumDistance;
    }

    // map converts the range of minimumDistance - maximumDistance to 0 - 100 as a float. map() floors all of it's arguments and 
    // returns an integer so in order to get a float from 0 - 100 as an output i multiplied each argument by 100 and then divided 
    // the output by 100.0 at the end. the decimal point in 100.0 is necessary 
    // to ensure it gives a float as map forces it's output to be an integer. 
    mappedDistance = map(distance * 100, minimumDistance * 100, maximumDistance * 100, 0, 10000) / 100.0;

    // Serial.print("Distance: ");
    // Serial.println(distance);
    // Serial.print("MAPPED Distance: ");
    // Serial.println(mappedDistance);

    lastPrintTime = millis();
  }

  // CORRECT. COMPLETED. 'D' LOGIC. 
  // if safety is off AND if turning right to any degree
  if (safetyIsOn == false && mappedDistance < mappedStraightAhead - mappedDeadZone) {
    if (mappedDistance <= (50 - mappedDeadZone) / 4) { // if mappedDeadZone is 10, then 10
      speedZone = 4;
      Keyboard.press('d');
    }
    else if (mappedDistance <= ((50 - mappedDeadZone) / 4) * 2) { // if mappedDeadZone is 10, then 20
      if (millis() - lastPressedTimeD > pressedDelayD) {
        lastPressedTimeD = millis();
        if (wasPressedD == false) {
          Keyboard.press('d');
          wasPressedD = true;
          if (speedZone < 3) {
            // This value is the windup of the steering wheel. The intention is to hold down the button until the steering wheel gets 
            // to the correct angle, and then tap the button consistently so that the steering wheel angle remains roughly the same. 
            // So the windup time should only happen when it needs winding up, hence, it doesn't need to if it's coming from a higher speedZone. 
            pressedDelayD = 500;
          }
          else {
            pressedDelayD = 150;
          }
        }
        else {
          Keyboard.release('d');
          wasPressedD = false;
          pressedDelayD = 100;
        }
        speedZone = 3;
      }
    }
    else if (mappedDistance <= ((50 - mappedDeadZone) / 4) * 3) { // if mappedDeadZone is 10, then 30
      if (millis() - lastSlightlyPressedTimeD > slightlyPressedDelayD) { // original values were on for 200 off for 200
        lastSlightlyPressedTimeD = millis();
        if (wasSlightlyPressedD == false) {
          Keyboard.press('d');
          wasSlightlyPressedD = true;
          if (speedZone < 2) {
            // This value is the windup of the steering wheel. The intention is to hold down the button until the steering wheel gets 
            // to the correct angle, and then tap the button consistently so that the steering wheel angle remains roughly the same. 
            // So the windup time should only happen when it needs winding up, hence, it doesn't need to if it's coming from a higher speedZone. 
            slightlyPressedDelayD = 200;
          }
          else {
            slightlyPressedDelayD = 100;
          }
        }
        else {
          Keyboard.release('d');
          wasSlightlyPressedD = false;
          slightlyPressedDelayD = 100;
        }
        speedZone = 2;
      }
    }
    else { // if mappedDeadZone is 10, then 40
      if (millis() - lastVerySlightlyPressedTimeD > 50) {
        lastVerySlightlyPressedTimeD = millis();
        if (wasVerySlightlyPressedD == false) {
          Keyboard.press('d');
          wasVerySlightlyPressedD = true;
        }
        else {
          Keyboard.release('d');
          wasVerySlightlyPressedD = false;
        }
      }
      speedZone = 1;
    }
  }
  else {
    speedZone = 0;
    Keyboard.release('d');
  }

  // START OF NEW 'A' LOGIC ===============================
  // if safety is off AND if turning left to any degree
  if (safetyIsOn == false && mappedDistance > mappedStraightAhead + mappedDeadZone) {
    if (mappedDistance >= 100 - ((50 - mappedDeadZone) / 4)) { // if mappedDeadZone is 10, then 90
      speedZone = 4;
      Keyboard.press('a');
    }
    else if (mappedDistance >= 100 - (((50 - mappedDeadZone) / 4) * 2)) { // if mappedDeadZone is 10, then 80
      if (millis() - lastPressedTimeA > pressedDelayA) {
        lastPressedTimeA = millis();
        if (wasPressedA == false) {
          Keyboard.press('a');
          wasPressedA = true;
          if (speedZone < 3) {
            // This value is the windup of the steering wheel. The intention is to hold down the button until the steering wheel gets 
            // to the correct angle, and then tap the button consistently so that the steering wheel angle remains roughly the same. 
            // So the windup time should only happen when it needs winding up, hence, it doesn't need to if it's coming from a higher speedZone. 
            pressedDelayA = 500;
          }
          else {
            pressedDelayA = 150;
          }
        }
        else {
          Keyboard.release('a');
          wasPressedA = false;
          pressedDelayA = 100;
        }
        speedZone = 3;
      }
    }
    else if (mappedDistance >= 100 - (((50 - mappedDeadZone) / 4) * 3)) { // if mappedDeadZone is 10, then 70
      if (millis() - lastSlightlyPressedTimeA > slightlyPressedDelayA) { // original values were on for 200 off for 200
        lastSlightlyPressedTimeA = millis();
        if (wasSlightlyPressedA == false) {
          Keyboard.press('a');
          wasSlightlyPressedA = true;
          if (speedZone < 2) {
            // This value is the windup of the steering wheel. The intention is to hold down the button until the steering wheel gets 
            // to the correct angle, and then tap the button consistently so that the steering wheel angle remains roughly the same. 
            // So the windup time should only happen when it needs winding up, hence, it doesn't need to if it's coming from a higher speedZone. 
            slightlyPressedDelayA = 200;
          }
          else {
            slightlyPressedDelayA = 100;
          }
        }
        else {
          Keyboard.release('a');
          wasSlightlyPressedA = false;
          slightlyPressedDelayA = 100;
        }
        speedZone = 2;
      }
    }
    else { // if mappedDeadZone is 10, then 60
      if (millis() - lastVerySlightlyPressedTimeA > 50) {
        lastVerySlightlyPressedTimeA = millis();
        if (wasVerySlightlyPressedA == false) {
          Keyboard.press('a');
          wasVerySlightlyPressedA = true;
        }
        else {
          Keyboard.release('a');
          wasVerySlightlyPressedA = false;
        }
      }
      speedZone = 1;
    }
  }
  else {
    speedZone = 0;
    Keyboard.release('a');
  }


  // END OF NEW 'A' LOGIC ===============================



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












