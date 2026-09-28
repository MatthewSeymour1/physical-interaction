/*

  Parts required:
  - one RGB LED
  - three 220 ohm resistors
*/  


const int redLEDPin = 11;   // LED connected to digital pin 11
const int greenLEDPin = 10;  // LED connected to digital pin 10
const int blueLEDPin = 9;  // LED connected to digital pin 9

int redValue = 0;    // value to write to the red LED
int greenValue = 0;  // value to write to the green LED
int blueValue = 0;   // value to write to the blue LED

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // set the digital pins as outputs
  
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
}

void loop() {
 

  redValue = 0;
  greenValue = 0;
  blueValue = 0;

  
  Serial.print("print out values \t red: ");
  Serial.print(redValue);
  Serial.print("\t green: ");
  Serial.print(greenValue);
  Serial.print("\t Blue: ");
  Serial.println(blueValue);

  /*
    Now that you have a usable value, it's time to PWM the LED.
  */
  //off
  Serial.println("LEDs off");
  analogWrite(redLEDPin, 255);
  analogWrite(greenLEDPin, 255);
  analogWrite(blueLEDPin, 255);
  delay(2000);
  Serial.println("red LED");
  analogWrite(redLEDPin, redValue);
  analogWrite(greenLEDPin, 255);
  analogWrite(blueLEDPin, 255);
  delay(2000);
  Serial.println("green LED");
  analogWrite(redLEDPin, 255);
  analogWrite(greenLEDPin, greenValue);
  analogWrite(blueLEDPin, 255);
  delay(2000);
  Serial.println("blue LED");
  analogWrite(redLEDPin, 255);
  analogWrite(greenLEDPin, 255);
  analogWrite(blueLEDPin, blueValue);

  delay(2000);
  
  Serial.println("mixed colour LED");
  analogWrite(redLEDPin, redValue);
  analogWrite(greenLEDPin, greenValue);
  analogWrite(blueLEDPin, blueValue);
  
  delay(2000);
}
