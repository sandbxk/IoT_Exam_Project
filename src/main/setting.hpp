
#pragma once

#include <string>

class Setting {

public:

    Setting(
        const std::string& ssid,
        const std::string& pass,
        const std::string& id,
        const std::string& broker,
        const std::string& topic,
        const size_t& port,
        const size_t& wifi_timeout,
        const std::string& token
    ) : m_ssid(ssid), m_pass(pass), 
        m_id(id), m_broker(broker), 
        m_topic(topic), m_port(port),
        m_wifi_connect_timeout_ms(wifi_timeout),
        m_token(token){};

    const std::string& ssid() const { return m_ssid; }
    const std::string& pass() const { return m_pass; }
    const std::string& id() const { return m_id; }
    const std::string& broker() const { return m_broker; }
    const std::string& token() const { return m_token; }
    const std::string& topic() const { return m_topic; }
    const size_t& port() const { return m_port; }
    const size_t& wifi_timeout() const { return m_wifi_connect_timeout_ms; }
    
private:
    std::string m_ssid;    // your network SSID (name)
    std::string m_pass;    // your network password (use for WPA, or use as key for 
    std::string m_id;   // your device ID
    std::string m_broker; // your MQTT broker
    std::string m_token; // your MQTT token
    std::string m_topic; // your MQTT topic
    size_t m_port; // your MQTT broker port

    size_t m_wifi_connect_timeout_ms = 10000;
};
