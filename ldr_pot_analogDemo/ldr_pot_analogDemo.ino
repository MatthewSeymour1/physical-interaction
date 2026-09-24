/*
  Analog Input

 
*/

int potPin = A0;   // select the input pin for the potentiometer
int ldrPin = A2;
int ledPin = 13;      // select the pin for the LED
int ldrValue = 0;  // variable to store the value coming from the ldr
int potValue = 0;  // variable to store the value coming from the pot


void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the value from the sensor:
  potValue = analogRead(potPin);
  ldrValue = analogRead(ldrPin);
  Serial.print(potValue);
  Serial.print(", ");
  Serial.println(ldrValue);
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  // stop the program for <sensorValue> milliseconds:
  delay(potValue);
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  // stop the program for <sensorValue> milliseconds:
  delay(potValue);
}
