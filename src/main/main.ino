
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

void setup() 
{
  Serial.begin(9600);

  client = std::make_unique<Client>(std::unique_ptr<Setting>(&settings));
  client->connect();
}

void loop() 
{
  if (client->pollIncoming()) {
    Serial.println(client->getMessage().c_str());
  }

  client->sendMessage("Hello World!");
  delay(1000);
}