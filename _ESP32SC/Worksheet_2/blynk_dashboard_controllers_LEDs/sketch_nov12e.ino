#define BLYNK_TEMPLATE_ID   "TMPL4Sf_QP176"
#define BLYNK_TEMPLATE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN    "KRy1U10O3Qt42O-rHJHIjwBzNmngynQT"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32_SSL.h>

char ssid[] = "Kate ";
char pass[] = "matthew0033";

const int LED_PINS[3] = { D1, D4, D5 };

bool blinkEnabled = false;
bool ledOn = false;
unsigned long lastToggle = 0;
const unsigned long BLINK_INTERVAL = 300;   // milliseconds

// V0: switch widget. On starts the LED sequence, off stops it.
BLYNK_WRITE(V0) {
  blinkEnabled = (param.asInt() == 1);
  Blynk.virtualWrite(V1, blinkEnabled ? 1 : 0);   // V1: LED widget shows 1 while the sequence runs
  Serial.println(blinkEnabled ? "Blink enabled" : "Blink disabled");
  if (!blinkEnabled) {
    for (int i = 0; i < 3; i++) digitalWrite(LED_PINS[i], LOW);
    ledOn = false;
  }
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);   // restore the switch state (and so V1) after a reconnection
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) pinMode(LED_PINS[i], OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  if (blinkEnabled && millis() - lastToggle >= BLINK_INTERVAL) {
    lastToggle = millis();
    ledOn = !ledOn;
    for (int i = 0; i < 3; i++) digitalWrite(LED_PINS[i], ledOn ? HIGH : LOW);
  }
}

