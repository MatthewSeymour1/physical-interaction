// LED, turns on and off when BOOT button on the board is pressed
const int LED_PIN = LED_BUILTIN;  // GPIO21, active low
const int BUTTON_PIN = 0;
bool ledOn = false;         // BOOT button, reads LOW when pressed
int stableState = HIGH;           // last confirmed button state
int lastReading = HIGH;           // most recent raw reading
unsigned long lastChange = 0;
const unsigned long DEBOUNCE_MS = 30;

 
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}
 
void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastReading) {
    lastChange = millis();        // the reading moved, so restart the timer
    lastReading = reading;
  }

  if ((millis() - lastChange) > DEBOUNCE_MS && reading != stableState) {
    stableState = reading;
    if(stableState == LOW) {
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      if(ledOn) {
        Serial.println("LED OFF");
      } else {
        Serial.println("LED ON");
      }
    }

   
    
  }


 
 
 
  // Serial.println(buttonReading);
  // digitalWrite(LED_PIN, LOW);   // LED on
  // delay(500);
  // digitalWrite(LED_PIN, HIGH);  // LED off
  // delay(500);
}