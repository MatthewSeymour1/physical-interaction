/*
  Question 3 - Spaceship dashboard
  -----------------------------------

  Wiring: two red LEDs and one green LED, each via a 220 ohm resistor,
  on Pins 3, 4 and 5.
  //The long leg of the LEDs goes to GND, the short leg connects to the PIN
  via a 220 ohm resistor. A push button on Pin 2, with a 10 K ohm
  pull-down resistor to GND on the same leg, and the other leg of the
  button connected to 5V.

  Behaviour: if the button is NOT pressed, the green LED is on and the
  red LEDs are off. If the button IS pressed, the green LED turns off
  and the two red LEDs blink alternately.
*/

// Create a global variable to hold the
// state of the switch. This variable is persistent
// throughout the program. Whenever you refer to
// switchstate, you're talking about the number it holds
int switchstate = 0;

void setup() {
  Serial.begin(9600);
  // declare the LED pins as outputs
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  // declare the switch pin as an input
  pinMode(2, INPUT);
}

void loop() {
  // read the value of the switch
  // digitalRead() checks to see if there is voltage
  // on the pin or not
  switchstate = digitalRead(2);
  Serial.println(switchstate);

  // if the button is not pressed
  // turn on the green LED and off the red LEDs
  if (switchstate == LOW) {
    digitalWrite(3, HIGH); // turn the green LED on
    digitalWrite(4, LOW);  // turn the red LED off
    digitalWrite(5, LOW);  // turn the red LED off
  }
  // this else is part of the above if() statement.
  // if the switch is not LOW (the button is pressed)
  // turn off the green LED and blink alternately the red LEDs

  else {
    digitalWrite(3, LOW);  // turn the green LED off
    digitalWrite(4, LOW);  // turn the red LED off
    digitalWrite(5, HIGH); // turn the red LED on
    // wait for a quarter second before changing the light
    delay(250);
    digitalWrite(4, HIGH); // turn the red LED on
    digitalWrite(5, LOW);  // turn the red LED off
    // wait for a quarter second before changing the light
    delay(250);
  }
}
