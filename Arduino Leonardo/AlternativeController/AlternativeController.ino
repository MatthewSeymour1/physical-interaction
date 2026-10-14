// Currently this contains just the code from the ultra_sonic_sensor file. and it just prints to the serial console thingy the distance. 
// Though first, try and get two simple buttons to emulate the keyboard for 'W' and 'S'. Make sure to have a safety. A button that you need to hold down for the keyboard emulation to work. To avoid arduino taking over your pc D:

// #include <Keyboard.h>


const int safetyButtonPin = 8;
bool safetyIsOn = true;
const int buttonPin = 7;

int buttonValue = HIGH;
int safetyButtonValue = HIGH;
int oldButtonValue = HIGH;
int oldSafetyButtonValue = HIGH;

unsigned long lastSafetyDebounceTime = 0;
unsigned long lastButtonDebounceTime = 0;
unsigned long debounceDelay = 50; 

unsigned long lastPrintTime = 0;
unsigned long printDelay = 100;

// This is the ultra_sonic_sensor variables.
  // const int trigPin = 5;
  // const int echoPin = 3;
  // float duration, distance;
// end of the ultra_sonic_sensor variables.
 
void setup() {
// This is the ultra_sonic_sensor code in SETUP.
  // pinMode(trigPin, OUTPUT);
  // pinMode(echoPin, INPUT);
// end of the ultra_sonic_sensor code in SETUP.

  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(safetyButtonPin, INPUT_PULLUP);
  // Keyboard.begin();
  Serial.println(safetyIsOn ? "safety IS on" : "safety NOT on");




}

void loop() {

// This is the ultra_sonic_sensor code in LOOP.
  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);

  // duration = pulseIn(echoPin, HIGH);
  // distance = (duration * .0343) / 2;
  // Serial.print("Distance: ");
  // Serial.println(distance);

  // // This effectively limits the speed of the entire void loop(). Lowering this number will allow it to run more often and without
  // delay(100);
// end of ultra_sonic_sensor code in LOOP. 

  int safetyButtonReading = digitalRead(safetyButtonPin);
  int buttonReading = digitalRead(buttonPin);

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

  if (buttonReading != oldButtonValue) {
    lastButtonDebounceTime = millis();
  }
  if ((millis() - lastButtonDebounceTime) > debounceDelay) {
    if (buttonReading != buttonValue) {
      buttonValue = buttonReading;
    }
  }
  if (safetyIsOn == false) {
    if (millis() - lastPrintTime > printDelay) {
      if (buttonValue == LOW) {
        Serial.println("button IS pressed");
      }
      else {
        Serial.println("button NOT pressed");
      }
      lastPrintTime = millis();
      }
  }
  oldButtonValue = buttonReading;


  

}











