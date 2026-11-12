#include <ESP32Servo.h>

const int POT_PIN = D2; // goes from 0 -> 4095 
const int SERVO_PIN = D0; // goes from 0 -> 179 degrees
int potValue;

int pos = 0;
Servo servo;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  servo.setPeriodHertz(50);              // standard servo frequency
  servo.attach(SERVO_PIN, 500, 2400);   // pulse range in microseconds
}

void loop() {

  int potReading = analogRead(POT_PIN); // goes from 0 -> 4095 
  potValue = potReading;
  int mappedPotValue = map(potValue, 0, 4095, 0, 179);
  mappedPotValue = constrain(mappedPotValue, 0, 179);

  Serial.print("servo degrees: ");
  Serial.println(mappedPotValue);
  servo.write(mappedPotValue);
  delay(15);

  // Serial.println("moving right");
  // for (pos = 0; pos <= 179; pos += 1) { // goes from 0 degrees to 180 degrees
  // // in steps of 1 degree
  
  // servo.write(pos);    // tell servo to go to position in variable 'pos
  // delay(15);             // waits 15ms for the servo to reach the position
	// }
  // Serial.println("moving left");
	// for (pos = 179; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
	// 	servo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(15);             // waits 15ms for the servo to reach the position
  // }
}
  