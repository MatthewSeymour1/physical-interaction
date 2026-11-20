// Piranha RGB LED: smooth rainbow fade
// Board: Arduino Uno
//
// Wiring:
//   LED red   -> 220 ohm resistor -> pin 9
//   LED green -> 220 ohm resistor -> pin 10
//   LED blue  -> 220 ohm resistor -> pin 11
//   LED common leg -> 5V (common anode) or GND (common cathode)
//
// How it works:
//   The rainbow is split into three stages, each 256 steps long:
//     Stage 1: red   fades to green  (passing through yellow)
//     Stage 2: green fades to blue   (passing through cyan)
//     Stage 3: blue  fades to red    (passing through purple)
//   That makes 768 steps in total, and then it starts again.

const int RED_PIN   = 9;    // LED pins must be PWM pins (marked ~)
const int GREEN_PIN = 10;
const int BLUE_PIN  = 11;
const int POT_PIN  = A3;
const int LDR_PIN  = A5;

int potValue = 0;
int blueLevel = 0;
int redLevel = 0;
int greenLevel = 0;

float adjustedBlueLevel = 0;
float adjustedGreenLevel = 0;
float adjustedRedLevel = 0;

int ldrValue = 0;
int ldrDark = 630; // replace with your darkest reading 
int ldrBright = 970; // replace with your brightest reading 

float lightIntensity = 0.00;

const int FADE_DELAY = 10;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  potValue = analogRead(POT_PIN); // 0 to 1023 
  ldrValue = map(constrain(analogRead(LDR_PIN), ldrDark, ldrBright), ldrDark, ldrBright, 0, 100);
  lightIntensity = ldrValue/100.0;

  blueLevel = map(potValue, 0, 1023, 0, 255); 
  redLevel = 255 - blueLevel; // red falls as blue rises 
  greenLevel = 255;

  adjustedBlueLevel = blueLevel * lightIntensity;
  adjustedRedLevel = redLevel * lightIntensity;
  adjustedGreenLevel = greenLevel * lightIntensity;

  setColour(adjustedRedLevel, adjustedGreenLevel, adjustedBlueLevel);

  delay(FADE_DELAY);
}

// Sets the LED colour using brightness values from 0 (off) to 255 (full)
void setColour(int red, int green, int blue) {

  // Inverts the values because of the common anode
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
