

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // say hi
  Serial.write("Hello World");
  // blink the LED
  digitalWrite(BUILTIN_LED, HIGH);
  delay(1000);
  digitalWrite(BUILTIN_LED, LOW);
  delay(1000);
}