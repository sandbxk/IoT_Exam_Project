#pragma once

#include "setting.hpp"

#include <string>
#include <memory>
#include <queue>

#include <WiFi.h>
#include <ArduinoMqttClient.h>

namespace IoT {

class Client {

public:
    Client(Setting* settings);
    ~Client();

    void connect();
    void disconnect();

    bool pollIncoming();
    void sendMessage(const std::string& message);
    void sendMessage(const std::string& message, const std::string& topic);

    bool hasMessage() const { return !this->m_messageQueue.empty(); }
    std::string getMessage();

    MqttClient* m_mqttClient = nullptr;

private:

    Setting* m_settings;
    WiFiClient m_wifiClient;
    
    std::queue<std::string> m_messageQueue = std::queue<std::string>();
};

}