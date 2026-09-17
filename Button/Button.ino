/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/digital/Button/
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
bool debouncer = false;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH && !debouncer) {
  //   // turn LED on:
  //   digitalWrite(ledPin, HIGH);
  // } else {
  //   // turn LED off:
  //   digitalWrite(ledPin, LOW);

    debouncer = true;


    matt();
    // sos();


    debouncer = false;
  }
}

void dot() {
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}

void dash() {
  digitalWrite(ledPin, HIGH);
  delay(300);
  digitalWrite(ledPin, LOW);
  delay(100);
}
//morse code translation
//m: --
//a: .-
//t: -
void matt() {
  m();
  a();
  t();
  t();
}

void m() {
  dash();
  dash();
  delay(100);
}

void a() {
  dot();
  dash();
  delay(100);
}

void t() {
  dash();
  delay(100);
}

//morse code translation
//s: ...
//o: ---
void sos() {
  s();
  o();
  s();
}

void s() {
  dot();
  dot();
  dot();
}

void o() {
  dash();
  dash();
  dash();
}


