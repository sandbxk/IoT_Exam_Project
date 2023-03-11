
// std includes
#include <string>
#include <memory>

// Internal includes
#include "arduino_secrets.h"
#include "mqtt.hpp"
#include "setting.hpp"

Setting settings = Setting
(
  SECRET_SSID, SECRET_PASS, 
  MQTT_CLIENT_ID, MQTT_BROKER, 
  MQTT_DEFAULT_TOPIC, MQTT_PORT, 
  WIFI_TIMEOUT, SECRET_FLESPI_TOKEN
);

std::unique_ptr<Client> client = nullptr;

#define MOTOR_PIN1 0
#define MOTOR_PIN2 14
#define SPIN_DURATION 2000

void setup() 
{
  Serial.begin(9600);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);

  client = std::make_unique<Client>(std::unique_ptr<Setting>(&settings));
  client->connect();
}

void loop() 
{
  std::string msg;

  if (client->pollIncoming()) {
    msg = client->getMessage().c_str();
  }

  if (msg == 'open')
  {
    spinMotor(true, SPIN_DURATION); //Spin clockwise for duration to open door
    spinMotor(false, SPIN_DURATION); //Spin counter-clockwise for duration to reset position

    client->sendMessage("door opened")
  }
  
  delay(1000);
}


void spinMotor(bool clockwise, int duration) {
  if (clockwise) 
  {
    digitalWrite(MOTOR_PIN1, HIGH);
    digitalWrite(MOTOR_PIN2, LOW);
    delay(duration);
    stopMotor()
  }
  else
  {
    digitalWrite(MOTOR_PIN1, LOW);
    digitalWrite(MOTOR_PIN2, HIGH);
    delay(duration);
    stopMotor()
  }
}

void stopMotor() {
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
}