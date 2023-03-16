#include "motor.hpp"                // from src/main/motor.hpp
#include <Arduino.h>                // from Arduino library

void IoT::Motor::forward()
{
    if (this->m_direction == DIRECTION::FORWARD) { return; }
    this->m_direction = DIRECTION::FORWARD;

    digitalWrite(this->m_d1Pin, HIGH);
    digitalWrite(this->m_d2Pin, LOW);
    digitalWrite(this->m_enablePin, HIGH);
}

void IoT::Motor::backward()
{
    if (this->m_direction == DIRECTION::BACKWARD) { return; }
    this->m_direction = DIRECTION::BACKWARD;

    digitalWrite(this->m_d1Pin, LOW);
    digitalWrite(this->m_d2Pin, HIGH);
    digitalWrite(this->m_enablePin, HIGH);
}

void IoT::Motor::stop()
{
    if (this->m_direction == DIRECTION::NONE) { return; }
    this->m_direction = DIRECTION::NONE;
    
    digitalWrite(this->m_enablePin, LOW);
}
