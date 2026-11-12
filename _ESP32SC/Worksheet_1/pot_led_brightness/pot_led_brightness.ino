const int POT_PIN = D0;
const int LED_PIN = D1;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);       // readings range from 0 to 4095
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int raw = analogRead(POT_PIN);
  int brightness = map(raw, 0, 4095, 0, 255);
  analogWrite(LED_PIN, brightness);
  Serial.println(raw);
  delay(20);
}