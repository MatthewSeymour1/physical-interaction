/*

  Parts required:
  - one RGB LED
  - three 220 ohm resistors
*/  


const int redLEDPin = 9;   // LED connected to digital pin 9
const int greenLEDPin = 11;  // LED connected to digital pin 10
const int blueLEDPin = 10;  // LED connected to digital pin 11

int redValue = 0;    // value to write to the red LED
int greenValue = 0;  // value to write to the green LED
int blueValue = 0;   // value to write to the blue LED

int fadeValue = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // set the digital pins as outputs
  
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
}

void loop() {
 

  redValue = 255;
  greenValue = 255;
  blueValue = 255;

  
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

  // fade in from min to max in increments of 5 points:
  Serial.println("red fade");
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    setColor(fadeValue,255,255);
    // wait for 30 milliseconds to see the dimming effect
    delay(100);
  }
  setColor(255,255,255);
  
  delay(1000);
  
  Serial.println("green fade");
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    setColor(255, fadeValue,255);
    // wait for 30 milliseconds to see the dimming effect
    delay(100);
  }
  setColor(255,255,255);
  delay(1000);

   Serial.println("blue fade");
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    setColor(255,255,fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(100);
  }
  setColor(255,255,255);
  delay(1000);
  
  

}

void setColor(int rValue, int gValue, int bValue) {
  analogWrite(redLEDPin, rValue);
  analogWrite(greenLEDPin, gValue);
  analogWrite(blueLEDPin, bValue);
  
}
