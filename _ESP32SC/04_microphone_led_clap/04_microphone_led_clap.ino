#include <ESP_I2S.h>

I2SClass I2S;
const int LED_PINS[3] = {D1, D4, D5};
const float THRESHOLDS[3] = {2000, 4000, 6000};
const int N = 256;                  // samples per block
int16_t buf[N];


const float CLAP_MIN_RMS = 1500.0;         // use your own measured clap values
const float JUMP_RATIO = 3.0;              // a clap is at least 3 times louder than the block before

bool ledOn = false;
float previousRms = 0;                     // loudness of the previous block


void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) pinMode(LED_PINS[i], OUTPUT);
  I2S.setPinsPdmRx(42, 41);         // clock pin, data pin (internal)
  //start the microphone
  if (!I2S.begin(I2S_MODE_PDM_RX, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
    Serial.println("Microphone failed to start");
    while (1) delay(1000);
  }
}

void loop() {
  size_t bytesRead = I2S.readBytes((char*)buf, sizeof(buf));
  int count = bytesRead / 2;        // two bytes per sample
  if (count == 0) return;

  float sum = 0;
  for (int i = 0; i < count; i++) sum += (float)buf[i] * buf[i];
  float rms = sqrtf(sum / count);

  bool loudEnough = rms > CLAP_MIN_RMS;                    // big enough to be a clap
  bool suddenJump = rms > previousRms * JUMP_RATIO;        // and it arrived suddenly
  
  //toggle the LED boolean
   if (loudEnough && suddenJump) {
    // ledOn = !ledOn;
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PINS[i],  rms > THRESHOLDS[i] ? HIGH : LOW );
    }
   }
  Serial.print(bytesRead);
  Serial.print(", ");
  Serial.print(ledOn);
  Serial.print(", ");
  Serial.println(rms);
  //light LED 1, 2 and 3 as rms passes three thresholds
  



  
  previousRms = rms;  
}
