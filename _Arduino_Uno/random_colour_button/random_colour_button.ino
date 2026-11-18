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

int potValue = 0;
int blueLevel = 0;
int redLevel = 0;

int ldrDark = 630; // replace with your darkest reading 
int ldrBright = 970; // replace with your brightest reading 

const int colours[10][3] = {
  {255, 0, 0},     // 0 Red
  {255, 80, 0},    // 1 Orange
  {0, 0, 255},     // 2 Blue
  {0, 255, 0},     // 3 Green
  {255, 255, 0},   // 4 Yellow
  {128, 0, 255},   // 5 Purple
  {255, 0, 255},   // 6 Magenta
  {0, 255, 255},   // 7 Cyan
  {255, 255, 255}, // 8 White
  {255, 105, 180}  // 9 Pink
};


 const int NUMCOLOURS = 10;

// Change this to match your LED:
// true  = common anode   (common leg connected to 5V)
// false = common cathode (common leg connected to GND)
const bool COMMON_ANODE = true;

// Time between steps in milliseconds.
// 10 gives one full rainbow roughly every 8 seconds.
// Smaller = faster, larger = slower.
const int FADE_DELAY = 10;

int position = 0;   // where we are in the rainbow: 0 to 767

void setup() {

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);

  randomSeed(analogRead(A4));

  int currentColour = random(NUMCOLOURS); // a number from 0 to 9
  Serial.print(currentColour);
  Serial.print( " ");
  setColour(colours[currentColour][0], colours[currentColour][1],  colours[currentColour][2]); // blue value 
}

void loop() {

/*
potValue = analogRead(A5); // 0 to 1023 
blueLevel = map(potValue, 0, 1023, 0, 255); 
redLevel = 255 - blueLevel; // red falls as blue rises 


int lightValue = constrain(analogRead(A5), ldrDark, ldrBright); 
int blueLevel = map(lightValue, ldrDark, ldrBright, 0, 255); 

  Serial.print("Pot Value: ");
  Serial.print(potValue);
  Serial.print("Blue Level: ");
  Serial.print(blueLevel);
  Serial.print("Red Level: ");
  Serial.println(redLevel);

  setColour(redLevel, 0, blueLevel);


  delay(FADE_DELAY);
  */
}

// Works out the colour for a position in the rainbow (0 to 767)
// and sends it to the LED.

// Sets the LED colour using brightness values from 0 (off) to 255 (full)
void setColour(int red, int green, int blue) {
  Serial.print("=== ");
  Serial.print(red);
  Serial.println(", ");
  Serial.print(green);
  Serial.println(", ");
  Serial.print(blue);
  // A common anode LED works "upside down": LOW means on.
  // So we flip the values before sending them to the pins.
  if (COMMON_ANODE) {
    red   = 255 - red;
    green = 255 - green;
    blue  = 255 - blue;
  }
 

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
