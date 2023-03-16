#pragma once

namespace IoT {

enum class DIRECTION
{
    NONE,
    FORWARD,
    BACKWARD
};

class Motor {
public:

    Motor(int pin0, int pin1, int pin2) : 
    m_enablePin(pin0), m_d1Pin(pin1), m_d2Pin(pin2) {};

    ~Motor() {};

    void forward();
    void backward();
    void stop();

    bool isMoving() const { return m_isMoving; }
    DIRECTION isForward() const { return m_direction;}

private:

    int m_enablePin = 0;
    int m_d1Pin = 0;
    int m_d2Pin = 0;

    bool m_isMoving = false;
    DIRECTION m_direction = DIRECTION::FORWARD;
};

}