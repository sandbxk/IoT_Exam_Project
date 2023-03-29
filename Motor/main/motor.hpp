#pragma once

#include <Arduino.h>
namespace IoT {

enum class DIRECTION
{
    NONE,
    FORWARD,
    BACKWARD
};

class Motor {
public:

    Motor(int pwm1, int pwm2, int dir1, int dir2)
    {
        this->m_pwm1Pin = pwm1;
        this->m_pwm2Pin = pwm2;
        this->m_d1Pin = dir1;
        this->m_d2Pin = dir2;
        
        // Configure PWM
        ledcSetup(0, 5000, 8);
        ledcAttachPin(m_pwm1Pin, 0);
        ledcAttachPin(m_pwm2Pin, 0);

        pinMode(m_d1Pin, OUTPUT);
        pinMode(m_d2Pin, OUTPUT);
    };

    ~Motor() {};

    void forward();
    void backward();
    void stop();
    void setSpeed(unsigned char speed) { this->m_speed = speed; }

    bool isMoving() const { return m_isMoving; }
    DIRECTION isForward() const { return m_direction;}

private:

    int m_pwm1Pin;
    int m_pwm2Pin;
    int m_d1Pin;
    int m_d2Pin;

    unsigned char m_speed = 255;

    bool m_isMoving = false;
    DIRECTION m_direction = DIRECTION::NONE;
};

}