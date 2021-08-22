#pragma once



class LightAlarm
{
public:
    LightAlarm(char const *ip, unsigned short port, unsigned char pwmPin);

    void init();
    void loop();

private:
    char const *const _ip;
    unsigned short const _port;
    unsigned char const _pinPwm;

    int _currentPwm = 0;
    int _targetPwm = 0;

    void checkCommand();
    void checkLight();

    void toggleLight();
    void startAlarm();
    void stopAlarm();

    unsigned long _lightLastTime = 0;
    unsigned long _commandLastTime = 0;
    unsigned long _period = 1000;

    unsigned long _disconnectTime = 0;

    void sendStatus() const;
    bool _justConnected = true;
};