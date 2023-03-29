#include "motor.hpp"                // from src/main/motor.hpp
#include <Arduino.h>                // from Arduino library

void IoT::Motor::forward()
{
    //if (this->m_direction == DIRECTION::FORWARD) { return; }
    //this->m_direction = DIRECTION::FORWARD;

    digitalWrite(this->m_d1Pin, HIGH);
    digitalWrite(this->m_d2Pin, LOW);

    ledcWrite(0, this->m_speed);
}

void IoT::Motor::backward()
{
    //if (this->m_direction == DIRECTION::BACKWARD) { return; }
    //this->m_direction = DIRECTION::BACKWARD;

    digitalWrite(this->m_d1Pin, LOW);
    digitalWrite(this->m_d2Pin, HIGH);
    
    ledcWrite(0, this->m_speed);
}

void IoT::Motor::stop()
{
    //if (this->m_direction == DIRECTION::NONE) { return; }
    //this->m_direction = DIRECTION::NONE;
    
    ledcWrite(0, 0);
}
