
// std includes
#include <string>

// Arduino & ESP32
#include <ArduinoMqttClient.h>
#include <WiFi.h>

// Internal includes
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
const std::string ssid = SECRET_SSID;    // your network SSID (name)
const std::string pass = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

const std::string id              = MQTT_CLIENT_ID;   // your device ID
const std::string broker          = MQTT_BROKER; // your MQTT broker
const std::string default_topic   = MQTT_DEFAULT_TOPIC; // your MQTT topic
const size_t port                 = MQTT_PORT; // your MQTT broker port

// function prototypes
void checkIncomingMessage();
void sendMessageForTopic(const std::string& topic, const std::string& message);

// global entities
WiFiClient wifiClient;
MqttClient* mqttClient = new MqttClient(wifiClient);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(SECRET_SSID);

  WiFi.begin(ssid.c_str());
  while (!WiFi.isConnected()) {
    // todo: add timeout and restart if not connected.
    delay(2000);
  }


  mqttClient->setId(id.c_str());
  mqttClient->setUsernamePassword(SECRET_FLESPI_TOKEN, "");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker.c_str());

  if (!mqttClient->connect(broker.c_str(), port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient->connectError());

    // todo restart if fail.
    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(default_topic.c_str());
  Serial.println();

  // subscribe to a topic
  mqttClient->subscribe(default_topic.c_str());

  // topics can be unsubscribed using:
  // mqttClient->unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(default_topic.c_str());
  Serial.println();
}

int count = 0;
unsigned long previousMillis = 0;
long interval = 1000;

void loop() {

  /*
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(count);

    // send message, the Print interface can be used to set the message contents
    sendMessageForTopic("arduino", "hello");

    Serial.println();

    count++;
  }
  */

  // check for incoming messages
  checkIncomingMessage();
}

void sendMessageForTopic(const std::string& topic, const std::string& message) {
  mqttClient->beginMessage(topic.c_str());
  mqttClient->print(message.c_str());
  mqttClient->endMessage();
}

void checkIncomingMessage() {
  // check for incoming messages
  auto len = mqttClient->parseMessage(); // calls poll internally

  if (len > 0) {
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient->messageTopic());
    Serial.print("', length ");
    Serial.print(len);
    Serial.println(" bytes:");

    uint8_t* message = new uint8_t[len + 1]; // +1 for null terminator
    memset(message, '\0', len + 1); // null initialize the buffer

    mqttClient->read(message, len);
    Serial.println((char*)message);
  }
}