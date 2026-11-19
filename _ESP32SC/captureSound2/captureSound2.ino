/*
 * WAV Recorder for Seeed XIAO ESP32S3 Sense
 * Version for ESP32 Arduino core 3.x (uses ESP_I2S.h and I2S.recordWAV)
 *
 * Requires PSRAM: Tools > PSRAM: "OPI PSRAM"
 *
 * Usage (Serial Monitor, 115200 baud, newline line ending):
 *   1. Type a label name, e.g. "hello", and press Enter
 *   2. Type "rec" and press Enter to record one sample
 *   3. Repeat "rec" for more samples, or type a new label
 *
 * Files are saved to the SD card as /label.1.wav, /label.2.wav, ...
 *
 * Adapted from M.Rovai (May 2023) and the original Seeed code
 */

#include <ESP_I2S.h>
#include "FS.h"
#include "SD.h"

#define RECORD_TIME   1      // seconds per sample
#define SAMPLE_RATE   16000
#define VOLUME_GAIN   2       // left shift: 0 = none, 1 = x2, 2 = x4
#define WAV_HEADER_SIZE 44

I2SClass I2S;
String baseFileName = "";
int fileNumber = 1;

void setup() {
  Serial.begin(115200);
  while (!Serial) ;

  // PDM microphone: clock on pin 42, data on pin 41
  I2S.setPinsPdmRx(42, 41);
  if (!I2S.begin(I2S_MODE_PDM_RX, SAMPLE_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
    Serial.println("Failed to initialise I2S!");
    while (1) ;
  }

  if (!SD.begin(21)) {
    Serial.println("Failed to mount SD card!");
    while (1) ;
  }

  Serial.println("Enter a label name:");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "rec") {
      if (baseFileName == "") {
        Serial.println("Enter a label name first.");
      } else {
        String fileName = "/" + baseFileName + "." + String(fileNumber) + ".wav";
        fileNumber++;
        recordWav(fileName);
      }
    } else if (command.length() > 0) {
      baseFileName = command;
      fileNumber = 1;
      Serial.println("Send rec to start recording label: " + baseFileName);
    }
  }
}

void recordWav(String fileName) {
  Serial.println("Recording...");

  // recordWAV returns a buffer containing a complete WAV file (header + audio)
  size_t wavSize = 0;
  uint8_t *wavBuffer = I2S.recordWAV(RECORD_TIME, &wavSize);
  if (wavBuffer == NULL) {
    Serial.println("Recording failed!");
    return;
  }

  // Optional volume boost, skipping the 44-byte header
  int16_t *samples = (int16_t *)(wavBuffer + WAV_HEADER_SIZE);
  size_t numSamples = (wavSize - WAV_HEADER_SIZE) / 2;
  for (size_t i = 0; i < numSamples; i++) {
    samples[i] <<= VOLUME_GAIN;
  }

  // Save to the SD card
  File file = SD.open(fileName.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing!");
  } else {
    if (file.write(wavBuffer, wavSize) != wavSize) {
      Serial.println("Write failed!");
    }
    file.close();
    Serial.printf("Saved %s (%u bytes)\n", fileName.c_str(), wavSize);
  }

  free(wavBuffer);
  Serial.println("Send rec for another sample or enter a new label.\n");
}
