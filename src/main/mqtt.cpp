#include "mqtt.hpp"
#include <ArduinoMqttClient.h>


Client::Client(std::unique_ptr<Setting> settings)
{
    this->m_mqttClient = std::make_unique<MqttClient>(this->m_wifiClient);
    this->m_settings = std::move(settings);

    this->m_mqttClient->setId(this->m_settings->id().c_str());
    this->m_mqttClient->setUsernamePassword(this->m_settings->token().c_str(), "");

}

Client::~Client()
{
    this->m_mqttClient.~unique_ptr();
    this->m_settings.~unique_ptr();
    this->m_wifiClient.stop();
}

void Client::connect()
{
    #define WIFI_WAIT 2000
    WiFi.begin(this->m_settings->ssid().c_str());
    size_t counter;

    // well defined double -> size_t conversion
    size_t maxCounter = static_cast<size_t>(std::round(this->m_settings->wifi_timeout() / WIFI_WAIT));
    
    while (!WiFi.isConnected()) {
        delay(WIFI_WAIT);
        counter++;
        if (counter > maxCounter) {
            ESP.restart();
        }
    }

    if (!this->m_mqttClient->connect(this->m_settings->broker().c_str(), this->m_settings->port())) {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(this->m_mqttClient->connectError());
        ESP.restart();
    }

    this->m_mqttClient->subscribe(this->m_settings->topic().c_str());
}

void Client::disconnect()
{
    this->m_wifiClient.stop();
    this->m_mqttClient->stop();
}

bool Client::pollIncoming()
{
    auto len = this->m_mqttClient->parseMessage();

    if (len > 0) {  
      std::unique_ptr<uint8_t> message = std::make_unique<uint8_t>(len + 1);
      memset(message.get(), '\0', len + 1);
      this->m_mqttClient->read(message.get(), len);

      this->m_messageQueue.push(std::string(reinterpret_cast<char*>(message.get())));
      return true;
    }

    return false;
}

void Client::sendMessage(const std::string &message)
{
    this->sendMessage(message, this->m_settings->topic());
}

void Client::sendMessage(const std::string &message, const std::string &topic)
{
    this->m_mqttClient->beginMessage(topic.c_str());
    this->m_mqttClient->write(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
    this->m_mqttClient->endMessage();
}

std::string Client::getMessage()
{ 
    auto message = this->m_messageQueue.front();
    this->m_messageQueue.pop();
    return message;
}
