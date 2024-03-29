#include "mqtt.hpp"
#include "ArduinoMqttClient.h"



IoT::Client::Client(Setting* settings)
{
    this->m_mqttClient = new MqttClient(this->m_wifiClient);
    this->m_settings = std::move(settings);

    this->m_mqttClient->setId(this->m_settings->id().c_str());
    this->m_mqttClient->setUsernamePassword(this->m_settings->token().c_str(), "");
}

IoT::Client::~Client()
{
    this->m_wifiClient.stop();
}

void IoT::Client::connect()
{
    #define WIFI_WAIT 2000
    WiFi.begin(this->m_settings->ssid().c_str());
    int counter = 0;
    int maxCounter = 8;
    int networks = WiFi.scanNetworks();

    Serial.println(networks);

    
    while (!WiFi.isConnected()) {
        if (counter < maxCounter) {
            Serial.println("Attempting to connect...");
            counter++;
            delay(1000);
        } else if (counter == maxCounter) {
            Serial.println("WiFi connection failed! Restarting...");
            ESP.restart();
        };
    }

    Serial.println("Connected to WiFi.");

    if (!this->m_mqttClient->connect(this->m_settings->broker().c_str(), this->m_settings->port())) {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(this->m_mqttClient->connectError());
        //ESP.restart();
    }

    Serial.println("Connected to MQTT broker.");

    this->m_mqttClient->subscribe(this->m_settings->topic().c_str());
}

void IoT::Client::disconnect()
{
    this->m_wifiClient.stop();
    this->m_mqttClient->stop();
}

bool IoT::Client::pollIncoming()
{
    auto len = this->m_mqttClient->parseMessage();

    if (len > 0) {  
      uint8_t* message = new uint8_t[len + 1];
      memset(message, '\0', len + 1);
      this->m_mqttClient->read(message, len);

      this->m_messageQueue.push(std::string(reinterpret_cast<char*>(message)));
      return true;
    }

    return false;
}

void IoT::Client::sendMessage(const std::string &message)
{
    this->sendMessage(message, this->m_settings->topic());
}

void IoT::Client::sendMessage(const std::string &message, const std::string &topic)
{
    this->m_mqttClient->beginMessage(topic.c_str());
    this->m_mqttClient->write(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
    this->m_mqttClient->endMessage();
}

std::string IoT::Client::getMessage()
{ 
    auto message = this->m_messageQueue.front();
    this->m_messageQueue.pop();
    return message;
}
