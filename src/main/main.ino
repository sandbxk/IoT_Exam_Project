
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

IoT::Client* client = nullptr;

#define MICROWAVE_SENSOR_PIN 0

void setup() 
{
  Serial.begin(9600);
  pinMode(MICROWAVE_SENSOR_PIN, INPUT_PULLDOWN);

  client = new IoT::Client(new Setting(settings));
  client->connect();
}

void loop() 
{

  clock_t lastSensed;
  clock_t currentTime = clock();

  if (lastSensed == NULL)
  {
    lastSensed = clock();
  }

  double runtime_diff_ms = (currentTime - lastSensed) * 1000. / CLOCKS_PER_SEC;
  

  int microwaveSensorValue = digitalRead(MICROWAVE_SENSOR_PIN);

  if (microwaveSensorValue == 1 && runtime_diff_ms > 5000) //Wait 5 seconds between sending a new MQTT message
  {
    client->sendMessage("open");
    delay(1000);
  }
  
  //client->sendMessage("Hello World!");
  delay(1000);
}