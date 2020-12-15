#pragma once



class LightAlarm
{
public:
    LightAlarm(unsigned short port, unsigned char pwmPin);

    void loop();

private:
    unsigned short const _port;
    unsigned char const _pinPwm;

    bool _inProgress = false;
    int _currentPwm = 0;

    void checkCommand();
    void checkLight();
    unsigned long _lastTime = 0;
};