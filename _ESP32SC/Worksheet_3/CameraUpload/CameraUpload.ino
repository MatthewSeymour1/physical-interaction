//Open up the ESP32 > WEbCameraServer example
//Replace the main sketch.ino with this


///
#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
// ===========================
// Select camera model in board_config.h
// (uncomment CAMERA_MODEL_XIAO_ESP32S3 there)
// ===========================
#include "board_config.h"

// ===========================
// WiFi credentials
// ===========================
const char *ssid = "Kate ";
const char *password = "matthew0033";

// ===========================
// Upload settings
// ===========================
// Use http:// for a PC on your network, https:// for a public host
//const char *uploadUrl = "http://192.168.1.50:5000/upload";
const char *uploadURL = "https://freshfruit.eu.pythonanywhere.com/upload";
const char *apiKey = "jpPythonUpload";
// ===========================
// Button settings
// ===========================
#define BUTTON_PIN 1  // D0 on the XIAO ESP32S3
const unsigned long DEBOUNCE_MS = 50;
int lastReading = HIGH;
int buttonState = HIGH;
unsigned long lastChange = 0;

bool cameraReady = false;

void startCameraServer();
void setupLedFlash();

// Sends one JPEG frame to the server. Returns true on HTTP 200.
bool uploadFrame(camera_fb_t *fb) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();  // skips certificate checking, acceptable for a hobby project

  HTTPClient http;
  http.begin(client, uploadURL);
  http.setTimeout(20000);
  http.addHeader("Content-Type", "image/jpeg");
  http.addHeader("X-Api-Key", apiKey);

  int code = http.POST(fb->buf, fb->len);
  Serial.printf("Upload response: %d\n", code);
  http.end();
  return code == 200;
}

// Switches to a larger size, captures, uploads, then restores the stream size.
void takePhoto() {
  sensor_t *s = esp_camera_sensor_get();

  s->set_framesize(s, FRAMESIZE_UXGA);
  delay(200);

  // Discard stale frames left over from the old resolution
  for (int i = 0; i < 2; i++) {
    camera_fb_t *old = esp_camera_fb_get();
    if (old) esp_camera_fb_return(old);
  }

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Capture failed");
  } else {
    Serial.printf("Captured %ux%u, %u bytes\n", fb->width, fb->height, fb->len);
    uploadFrame(fb);
    esp_camera_fb_return(fb);  // always return the buffer
  }

  s->set_framesize(s, FRAMESIZE_QVGA);
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // With PSRAM, use higher JPEG quality and two frame buffers
  if (psramFound()) {
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
  }
  cameraReady = true;

  sensor_t *s = esp_camera_sensor_get();
  // OV3660 starts flipped vertically and slightly oversaturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  // Lower frame size for a faster initial stream
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(LED_GPIO_NUM)
  setupLedFlash();
#endif

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to view the stream");
  Serial.println("Press the button on D0 to capture and upload a photo");
}

void loop() {
  if (!cameraReady) {
    delay(1000);
    return;
  }

  // Debounced button handling
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastReading) {
    lastChange = millis();
    lastReading = reading;
  }
  if ((millis() - lastChange) > DEBOUNCE_MS && reading != buttonState) {
    buttonState = reading;
    if (buttonState == LOW) {  // pressed (INPUT_PULLUP reads LOW)
      takePhoto();
    }
  }
  delay(5);
}
