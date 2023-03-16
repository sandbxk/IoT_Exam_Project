
#include <memory>                 // from C++ library
#include "WString.h"              // from DFRobot esp32 library
#include "arduino_secrets.h"      // from src/main/arduino_secrets.hpp
#include "mqtt.hpp"               // from src/main/mqtt.hpp
#include "motor.hpp"              // from src/main/motor.hpp
#include "setting.hpp"            // from src/main/setting.hpp

Setting settings = Setting
(
  SECRET_SSID, SECRET_PASS, 
  MQTT_CLIENT_ID, MQTT_BROKER, 
  MQTT_DEFAULT_TOPIC, MQTT_PORT, 
  WIFI_TIMEOUT, SECRET_FLESPI_TOKEN
);

#define ENABLE_PIN 17
#define D1_PIN 0
#define D2_PIN 14

IoT::Client* client = nullptr;
IoT::Motor motor = IoT::Motor(ENABLE_PIN, D1_PIN, D2_PIN);

void setup()
{
  Serial.begin(9600);
  client = new IoT::Client(new Setting(settings));
  
  client->connect();

  client->subscribe(MQTT_DEFAULT_TOPIC);
  client->subscribe("door/open");
  client->subscribe("door/close");
}

void loop()
{
  if (client->pollIncoming()) {
    Serial.println(client->getMessage().payload().c_str());

    if (client->getMessage().topic() == "door/open") {
      motor.forward();
      delay(2000); // todo: motor run until trigger
      motor.stop();
      
      client->sendMessage("opened");
    }
    else if (client->getMessage().topic() == "door/close") {
      motor.backward();
      delay(2000); // todo: motor run until trigger
      motor.stop();

      client->sendMessage("opened");
    }
  }
}