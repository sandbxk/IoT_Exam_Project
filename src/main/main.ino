
#define MOTOR_1A 0
#define MOTOR_2A 14


// the setup routine runs once when you press reset:
void setup() {
  pinMode(MOTOR_1A, OUTPUT);
  pinMode(MOTOR_2A, OUTPUT);
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {

  Serial.write("Hello World!");
  

  digitalWrite(MOTOR_1A, HIGH);
  digitalWrite(MOTOR_2A, LOW);
  
  Serial.write("Hello World!");
  delay(5000);
  digitalWrite(MOTOR_1A, LOW);
  digitalWrite(MOTOR_2A, HIGH);
  delay(5000);
  


  
}