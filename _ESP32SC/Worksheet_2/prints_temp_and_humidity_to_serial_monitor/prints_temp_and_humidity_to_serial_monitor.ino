#include <DHT.h>

#define DHT_PIN  D3
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();   // degrees Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Sensor read failed");
  } else {
    Serial.println("Temperature: " + String(temperature, 1) + " C");
    Serial.println("Humidity: " + String(humidity, 0) + " %");
  }
  delay(2000);
}

