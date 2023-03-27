
#include <memory>                 // from C++ library
#include "WString.h"              // from DFRobot esp32 library
#include "arduino_secrets.h"      // from src/main/arduino_secrets.hpp
#include "setting.hpp"            // from src/main/setting.hpp
#include "mqtt.hpp"               // from src/main/mqtt.hpp
#include "motor.hpp"              // from src/main/motor.hpp

Setting settings = Setting
(
  SECRET_SSID, SECRET_PASS, 
  MQTT_CLIENT_ID, MQTT_BROKER, 
  MQTT_DEFAULT_TOPIC, MQTT_PORT, 
  WIFI_TIMEOUT, SECRET_FLESPI_TOKEN
);

#define PWM1 16
#define PWM2 17

#define D1_PIN 0
#define D2_PIN 14

IoT::Client* mqttClient = new IoT::Client(&settings);
IoT::Motor* motor = new IoT::Motor(PWM1, PWM2, D1_PIN, D2_PIN);

void setup()
{
  Serial.begin(9600);
  
  mqttClient->connect();
  mqttClient->subscribe("door/open");
  mqttClient->subscribe("door/close");

  motor->setSpeed(200); // 0 -> 255
}

void loop()
{
  if (mqttClient->isConnected()) 
  {
    if (mqttClient->pollIncoming()) {
      auto message = mqttClient->getMessage();
      
      if (message.topic().equals("door/open")) {
        Serial.println("Motor forward");
        motor->forward();
        delay(200); // todo: motor run until trigger
        motor->stop();      
      }
      else if (message.topic().equals("door/close")) {
        Serial.println("Motor backward");
        motor->backward();
        delay(20); // todo: motor run until trigger
        motor->stop();
      }
    }
  }
  else 
  {
    Serial.println("Reconnecting...");
    mqttClient->connect();
  }
}

