#include "lightalarm.h"

#include "Arduino.h"
#include <WiFiClient.h>

namespace{
WiFiClient _client;
}

LightAlarm::LightAlarm(char const *ip, unsigned short port, unsigned char pwmPin)
    : _ip(ip),
      _port(port),
      _pinPwm(pwmPin)
{}

void LightAlarm::init()
{
    pinMode(_pinPwm, OUTPUT);
    digitalWrite(_pinPwm, LOW);
    analogWriteFreq(300);
    // analogWriteRange(1023);
}

void LightAlarm::loop()
{
    checkLight();
    checkCommand();
}

void LightAlarm::checkCommand()
{
    auto currentTime = millis();
    if ( currentTime - _commandLastTime < 1000 ) return;
    _commandLastTime = currentTime;

    Serial.println("Checking command...");

    if ( _client.connected() )
    {
        Serial.println("Client connected");
        if ( !_client.available() ) return;

        Serial.println("Data received!");
        char command = _client.read();
        Serial.printf("Command: %c\n", command);

        if ( command == 'T' ) toggleLight();
        else if (command == 'S') startAlarm();
        else if (command == 'F') stopAlarm();
    }
    else _client.connect(_ip, _port);
}

void LightAlarm::checkLight()
{
    auto currentTime = millis();
    if ( currentTime - _lightLastTime < _period ) return;
    _lightLastTime = currentTime;

    if (_currentPwm < _targetPwm) ++_currentPwm;
    else if (_currentPwm > _targetPwm) --_currentPwm;
    else return;

    Serial.println(_currentPwm);
    analogWrite(_pinPwm, _currentPwm);
}

void LightAlarm::toggleLight()
{
    _period = 1;

    if ( _currentPwm > 0 ) _targetPwm = 0;
    else _targetPwm = 1023;

    _client.write(_targetPwm > 0);
}

constexpr unsigned long alarmPeriod = 4000;
void LightAlarm::startAlarm()
{
    _period = alarmPeriod;
    _targetPwm = 1023;

    _client.write(_targetPwm > 0);
}

void LightAlarm::stopAlarm()
{
    if (_period == alarmPeriod) toggleLight();
}
