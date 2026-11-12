const int LDR_PIN = D1;
const int BUZZER_PIN = D3;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  int light = analogRead(LDR_PIN);
  int pitch = map(light, 2000, 3500, 200, 2000);   // frequency in hertz
  int constrainedPitch constrain(pitch,200,2000);
  if(constrainedPitch < 1800) {
    tone(BUZZER_PIN, constrainedPitch);
  }
  else {
    noTone(BUZZER_PIN);
  }
  Serial.print(light);
  Serial.print(", ");
  Serial.println(constrainedPitch);
  delay(20);
}