/*
  Blynk button runs a three-LED sequence, plus DHT11 temperature and humidity
  Board: Seeed Studio XIAO ESP32S3 (Sense)

  Based on the Blynk quickstart template.
    V0 : switch/button widget. Turning it on starts the LED sequence.
    V1 : LED widget. Shows 1 while the sequence is running.
    V4 : temperature from the DHT11 (Double, degrees C)
    V5 : humidity from the DHT11 (Double, percent)

  LEDs (each anode through 220 ohm, cathodes to GND):
    LED 1: D1,  LED 2: D4,  LED 3: D5

  DHT11 (3-pin module):
    VCC  -> 3V3
    GND  -> GND
    DATA -> D2

  Libraries: Blynk, DHT sensor library (Adafruit), Adafruit Unified Sensor
*/
/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL4Sf_QP176" //Blynk Template ID
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"     //Blynk Template Name
#define BLYNK_AUTH_TOKEN            "KRy1U10O3Qt42O-rHJHIjwBzNmngynQT" //Auth Toked

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32_SSL.h>   // SSL version: blynk.cloud on port 443
#include <DHT.h>

// Your WiFi credentials.
char ssid[] = "Kate ";
char pass[] = "matthew0033";

const int LED_PINS[3] = {D1, D4, D5};

// ---- DHT11 settings ----
#define DHT_PIN  D3
#define DHT_TYPE DHT11
const unsigned long SENSOR_INTERVAL_MS = 5000;   // DHT11 cannot be read faster than about 1 Hz

DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;

// ---- Sequence state (non-blocking, so Blynk keeps running) ----
bool running = false;
int step = 0;
unsigned long lastStep = 0;
const unsigned long STEP_MS = 150;
// Chase 1,2,3,2,1 (5 steps), then three flashes (6 steps: on, off, on, off, on, off)
const int CHASE_PATTERN[5] = {0, 1, 2, 1, 0};
const int CHASE_STEPS = 5;
const int FLASH_STEPS = 6;
const int TOTAL_STEPS = CHASE_STEPS + FLASH_STEPS;

void setAll(bool on) {
  for (int i = 0; i < 3; i++) digitalWrite(LED_PINS[i], on ? HIGH : LOW);
}

void startSequence() {
  if (running) return;
  running = true;
  step = 0;
  lastStep = 0;
  Blynk.virtualWrite(V1, 1);          // LED widget on while running
}

void updateSequence() {
  if (!running) return;
  if (millis() - lastStep < STEP_MS) return;
  lastStep = millis();

  if (step < CHASE_STEPS) {
    setAll(false);
    digitalWrite(LED_PINS[CHASE_PATTERN[step]], HIGH);
  } else if (step < TOTAL_STEPS) {
    setAll((step - CHASE_STEPS) % 2 == 0);
  } else {
    // Finished: LEDs off, reset the switch and the LED widget on the dashboard
    setAll(false);
    running = false;
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 0);
    return;
  }
  step++;
}

// Read the DHT11 and send the values to Blynk
void sendSensorData() {
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();   // degrees Celsius

  // A failed read returns NaN, so skip the update rather than send rubbish
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT11 read failed. Check wiring and pin.");
    return;
  }

  Serial.printf("Temperature: %.1f C, Humidity: %.0f %%\n", temperature, humidity);
  Blynk.virtualWrite(V4, temperature);
  Blynk.virtualWrite(V5, humidity);
}

// Called every time the V0 widget changes
BLYNK_WRITE(V0) {
  if (param.asInt() == 1) startSequence();
}

// Called when the device connects to Blynk Cloud
BLYNK_CONNECTED() {
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);
}

void setup() {
  Serial.begin(115200);
  Serial.println("hello Blynk");

  for (int i = 0; i < 3; i++) pinMode(LED_PINS[i], OUTPUT);
  setAll(false);

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Read the sensor on a timer so it never blocks the LED sequence
  timer.setInterval(SENSOR_INTERVAL_MS, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
  updateSequence();
  // Avoid delay() here, so the sequence and Blynk both keep running.
}
