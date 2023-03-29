
#pragma once

#include "WString.h"                // from DFRobot esp32 library

class Setting {

public:

    Setting(
        const String& ssid,
        const String& pass,
        const String& id,
        const String& broker,
        const String& topic,
        const size_t& port,
        const size_t& wifi_timeout,
        const String& token
    ) : m_ssid(ssid), m_pass(pass), 
        m_id(id), m_broker(broker), 
        m_topic(topic), m_port(port),
        m_wifi_connect_timeout_ms(wifi_timeout),
        m_token(token){};

    const String& ssid() const { return m_ssid; }
    const String& pass() const { return m_pass; }
    const String& id() const { return m_id; }
    const String& broker() const { return m_broker; }
    const String& token() const { return m_token; }
    const String& topic() const { return m_topic; }
    const size_t& port() const { return m_port; }
    const size_t& wifi_timeout() const { return m_wifi_connect_timeout_ms; }
    
private:
    String m_ssid;    // your network SSID (name)
    String m_pass;    // your network password (use for WPA, or use as key for 
    String m_id;   // your device ID
    String m_broker; // your MQTT broker
    String m_token; // your MQTT token
    String m_topic; // your MQTT topic
    size_t m_port; // your MQTT broker port

    size_t m_wifi_connect_timeout_ms = 10000;
};
