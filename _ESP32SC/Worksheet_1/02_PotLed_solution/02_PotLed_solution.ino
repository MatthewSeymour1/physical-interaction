const int POT_PIN = D0;
const int LED_PIN = D1;

const int N = 10;                    // number of readings to average
int readings[N];                     // circular buffer of the last N readings
int readIndex = 0;                       // where the next reading is stored
long total = 0;                      // running sum of the buffer
int filled = 0;                      // how many slots hold real readings so far

const bool USE_CORRECTION = true;    // extension: square the normalised value

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);       // readings range from 0 to 4095 -> 2^ 12 values
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int raw = analogRead(POT_PIN);
  //smoothing code - circular buffer
  total -= readings[readIndex];          // remove the oldest reading from the sum
   readings[readIndex] = raw;             // store the newest one in its place
   total += raw;
   readIndex = (readIndex + 1) % N;           // wrap round to 0 after N - 1 for circular buffer
   if (filled < N) filled++;          // average only real readings at start-up

   int smoothed = total / filled;

   Serial.print(raw);                 // two values on one line, separated by a comma,
   Serial.print(",");                 // so the Serial Plotter draws two lines
   Serial.print(smoothed);
   Serial.print(","); 

  float level = smoothed / 4095.0;               // normalised to 0.0 to 1.0
  
  //simple gamma correction for LED
  if (USE_CORRECTION) level = level * level;  
  int duty = (int)(level * 255 + 0.5);   // scale once, round to nearest  ->squaring approximates gamma
  duty = constrain(duty, 0, 255);
  Serial.println(duty);
  analogWrite(LED_PIN, duty);

  delay(20);
}