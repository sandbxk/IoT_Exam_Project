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
    this->m_wifiClient.stop(); /// this is not necessary, but it's good practice

    /// free memory allocated by new operator
    delete this->m_mqttClient;
    delete this->m_settings;

    /// queue is automatically deleted (RAII) - stack allocated
    /// wifiClient is automatically deleted (RAII) - stack allocated
}

void IoT::Client::connect()
{
    #define WIFI_WAIT 2000
    WiFi.begin(this->m_settings->ssid().c_str());
    size_t counter;

    /// well defined double -> size_t conversion
    size_t maxCounter = static_cast<size_t>(std::round(this->m_settings->wifi_timeout() / WIFI_WAIT));
    
    while (!WiFi.isConnected()) {
        //delay(WIFI_WAIT);
        counter++;
        if (counter > maxCounter) {
            Serial.println("WiFi connection failed!");
            //ESP.restart();
        }
    }

    Serial.println("Connected to WiFi.");

    if (!this->m_mqttClient->connect(this->m_settings->broker().c_str(), this->m_settings->port())) {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(this->m_mqttClient->connectError());
        //ESP.restart();
    }

    Serial.println("Connected to MQTT broker.");

    /// subscribe to the default topic
    this->m_mqttClient->subscribe(this->m_settings->topic().c_str());
}

void IoT::Client::disconnect()
{
    this->m_mqttClient->stop(); /// tell the broker we're disconnecting
    this->m_wifiClient.stop(); /// disconnect wifi
}

bool IoT::Client::pollIncoming()
{
    /// check if there's any incoming message (calls poll() internally)
    auto len = this->m_mqttClient->parseMessage();

    if (len > 0) 
    {
        /// allocate memory for the message
        uint8_t* message = new uint8_t[len + 1];
        memset(message, '\0', len + 1);

        /// read the message into the buffer 
        this->m_mqttClient->read(message, len);
        
        /// push the message into the queue (copy)
        this->m_messageQueue.push(Message(
            String(reinterpret_cast<char*>(message)), 
            this->m_mqttClient->messageTopic()
        ));

        /// scope ends, memory is automatically freed (RAII - new operator)
        return true;
    }

    return false;
}

void IoT::Client::sendMessage(const String &message, const String &topic)
{
    this->m_mqttClient->beginMessage(topic.c_str());
    this->m_mqttClient->write(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
    this->m_mqttClient->endMessage();
}

void IoT::Client::sendMessage(const String &message)
{
    this->sendMessage(message, this->m_settings->topic());
}

void IoT::Client::sendMessage(const Message &message)
{
    this->sendMessage(message.payload(), message.topic());
}

IoT::Message IoT::Client::getMessage()
{ 
    /// copy the message from the queue
    auto message = this->m_messageQueue.front();
    
    /// remove the message from the queue
    this->m_messageQueue.pop();

    /// return the message
    return message;
}
