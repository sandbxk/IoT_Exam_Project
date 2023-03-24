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
    delete this->m_mqttClient;
    delete this->m_settings;
}

bool connectWifi(const char* ssid) {
    if (WiFi.isConnected()) { return true; }

    WiFi.begin(ssid);
    size_t counter;
    size_t maxCounter = 5;
    
    while (!WiFi.isConnected()) {
        delay(2000);
        counter++;
        if (counter > maxCounter) {
            return false;
        }
    }

    Serial.println("Connected to WiFi.");

    return true;
}

void IoT::Client::connect()
{
    if (!connectWifi(this->m_settings->ssid().c_str()))
    {
        Serial.println("Failed to connect to WiFi.");
        this->connect();
    }

    if (!this->m_mqttClient->connect(this->m_settings->broker().c_str(), this->m_settings->port())) {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(this->m_mqttClient->connectError());
    }

    Serial.println("Connected to MQTT broker.");

    // incase we reconnect, we need to resubscribe to all the topics
    for (auto topic : this->m_subscribedTopics)
    {
        this->subscribe(topic);
    }
}

void IoT::Client::disconnect()
{
    this->m_mqttClient->stop(); /// tell the broker we're disconnecting
    this->m_wifiClient.stop(); /// disconnect wifi
}

void IoT::Client::subscribe(const String &topic)
{
    if (this->m_mqttClient->subscribe(topic.c_str()))
    {
        this->m_subscribedTopics.push_back(topic);
        Serial.print("Subscribed to topic: ");
        Serial.println(topic);
    }
    else
    {
        Serial.print("Failed to subscribe to topic: ");
        Serial.println(topic);
    }
}

void IoT::Client::unsubscribe(const String &topic)
{
    if (this->m_mqttClient->unsubscribe(topic.c_str()))
    {
        // vector erase-remove idiom
        this->m_subscribedTopics.erase(std::remove(this->m_subscribedTopics.begin(), this->m_subscribedTopics.end(), topic), this->m_subscribedTopics.end());
        Serial.print("Unsubscribed from topic: ");
        Serial.println(topic);
    }
    else
    {
        Serial.print("Failed to unsubscribe from topic: ");
        Serial.println(topic);
    }
}

bool IoT::Client::pollIncoming()
{
    /// check if there's any incoming message (calls poll() internally)
    auto len = this->m_mqttClient->parseMessage();

    if (len > 0) 
    {
        Serial.print("Message arrived [");
        Serial.print(this->m_mqttClient->messageTopic());
        Serial.print("]: ");

        /// push the message into the queue (copy)
        this->m_messageQueue.push(Message(
            this->m_mqttClient->readString(),
            this->m_mqttClient->messageTopic()
        ));

        return true;
    }

    return false;
}

void IoT::Client::sendMessage(const String &message, const String &topic)
{
    this->m_mqttClient->beginMessage(topic.c_str());
    this->m_mqttClient->write((reinterpret_cast<const uint8_t*>(message.c_str()), message.length()));
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
