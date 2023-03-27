
// std includes
#include <string>
#include <memory>
#include <Adafruit_NeoPixel.h>

// Internal includes
#include "arduino_secrets.h"
#include "mqtt.hpp"
#include "setting.hpp"

#define DATA_PIN 5
#define NUM_LEDS 1

Adafruit_NeoPixel onboardLED = Adafruit_NeoPixel(1, BUILTIN_LED);
Adafruit_NeoPixel rgbLED = Adafruit_NeoPixel(1, DATA_PIN, NEO_GRB + NEO_KHZ800);


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


void setColor(u8_t r, u8_t g, u8_t b) {
  rgbLED.clear();
  rgbLED.setPixelColor(0, rgbLED.Color(r, g, b));
  rgbLED.show();
}

void setup() 
{
  Serial.begin(9600);
  pinMode(MICROWAVE_SENSOR_PIN, INPUT_PULLDOWN);
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  rgbLED.begin();
  rgbLED.setBrightness(5);
  setColor(255, 0, 0);

  client = new IoT::Client(new Setting(settings));
  client->connect();

  onboardLED.begin();
  pinMode(MOTION_SENSOR_PIN, INPUT_PULLDOWN);
  WiFi.setSleep(false);
}

void loop() 
{ 
  if (!WiFi.isConnected())
  {
    Serial.println("WiFi not connected");
    client->connect();
    Serial.println("WiFi connected");
    setColor(255, 0, 255);
  }

  if (!client->m_mqttClient->connected())
  {
    Serial.println("MQTT not connected");
    client->connect();
    Serial.println("MQTT connected");
    setColor(255, 255, 0);
  }

  onboardLED.setPixelColor(0, onboardLED.Color(0, 0, 255));
  onboardLED.show();
  setColor(0, 250, 0);

  MOTION_SENSOR_OUTPUT = digitalRead(MOTION_SENSOR_PIN);

  if (MOTION_SENSOR_OUTPUT == 1 && MOTION_DETECTED == 0)
  {
    client->sendMessage("Motion Detected", "door/open");
    Serial.println("MOVEMENT DETECTED");
    MOTION_DETECTED = 1;
    digitalWrite(ONBOARD_LED, HIGH);
    setColor(0, 0, 250);
    delay(1000);
  }
  else if (MOTION_SENSOR_OUTPUT == 0 && MOTION_DETECTED == 1)
  {
    MOTION_DETECTED = 0;
    Serial.println("NO MOVEMENT");
    client->sendMessage("No Movement", "door/close");
    digitalWrite(ONBOARD_LED, LOW);
    setColor(0, 250, 0);
    delay(1000);
  }
}
