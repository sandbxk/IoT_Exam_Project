
// std includes
#include <string>
#include <memory>

// Internal includes
#include "arduino_secrets.h"
#include "mqtt.hpp"
#include "setting.hpp"

int MOTION_SENSOR_PIN = 0;
int MOTION_SENSOR_OUTPUT = 0; 
int MOTION_DETECTED = 0;

Setting settings = Setting
(
  SECRET_SSID, SECRET_PASS, 
  MQTT_CLIENT_ID, MQTT_BROKER, 
  MQTT_DEFAULT_TOPIC, MQTT_PORT, 
  WIFI_TIMEOUT, SECRET_FLESPI_TOKEN
);

IoT::Client* client = nullptr;

#define MICROWAVE_SENSOR_PIN 0
#define ONBOARD_LED BUILTIN_LED

void setup() 
{
  Serial.begin(9600);
  pinMode(MICROWAVE_SENSOR_PIN, INPUT_PULLDOWN);
  pinMode(ONBOARD_LED, OUTPUT);

  client = new IoT::Client(new Setting(settings));
  client->connect();

  pinMode(MOTION_SENSOR_PIN, INPUT_PULLDOWN);
}

void loop() 
{
  MOTION_SENSOR_OUTPUT = digitalRead(MOTION_SENSOR_PIN);

  if (MOTION_SENSOR_OUTPUT == 1 && MOTION_DETECTED == 0)
  {
    client->sendMessage("Motion Detected", "motiondetector");
    Serial.println("MOVEMENT DETECTED");
    MOTION_DETECTED = 1;
    digitalWrite(ONBOARD_LED, HIGH);
    delay(1000);
  }
  else if (MOTION_SENSOR_OUTPUT == 0 && MOTION_DETECTED == 1)
  {
    MOTION_DETECTED = 0;
    Serial.println("NO MOVEMENT");
    digitalWrite(ONBOARD_LED, LOW);
    delay(1000);
  }
}