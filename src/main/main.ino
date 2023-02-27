
#define MICROWAVE_SENSOR_PIN 0

// the setup routine runs once when you press reset:
void setup() {
  pinMode(MICROWAVE_SENSOR_PIN, INPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  int microwaveSensorValue = digitalRead(MICROWAVE_SENSOR_PIN);
  Serial.println(microwaveSensorValue);
  delay(1000);
}