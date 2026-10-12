// Currently this contains just the code from the ultra_sonic_sensor file. and it just prints to the serial console thingy the distance. 
// Though first, try and get two simple buttons to emulate the keyboard for 'W' and 'S'. Make sure to have a safety. A button that you need to hold down for the keyboard emulation to work. To avoid arduino taking over your pc D:

const int trigPin = 5;
const int echoPin = 3;
float duration, distance;
 
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // This effectively limits the speed of the entire void loop(). Lowering this number will allow it to run more often and without
  delay(100);
}
