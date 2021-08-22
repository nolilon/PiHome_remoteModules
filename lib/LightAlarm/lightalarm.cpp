#include "lightalarm.h"

#include "Arduino.h"
#include <WiFiClient.h>

namespace{
WiFiClient _client;
}

unsigned long linearBrightnessPwm(unsigned long targetBrightness);

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
        _disconnectTime = 0;
        Serial.println("Client connected");
        if ( !_client.available() ) return;

        Serial.println("Data received!");
        char command = _client.read();
        Serial.printf("Command: %c\n", command);

        if ( command == 'T' ) toggleLight();
        else if (command == 'S') startAlarm();
        else if (command == 'F') stopAlarm();
    }
    else 
    {
        _client.connect(_ip, _port);
        if (_disconnectTime == 0) _disconnectTime = millis();
        if (millis() - _disconnectTime > 300000ul) ESP.restart();
    }
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
    analogWrite(_pinPwm, linearBrightnessPwm(_currentPwm));
}

void LightAlarm::toggleLight()
{
    _period = 1;

    if ( _currentPwm > 0 ) _targetPwm = 0;
    else _targetPwm = 1023;

    _client.write(_targetPwm > 0);
}

constexpr unsigned long lightUpMinutes = 60;
constexpr unsigned long alarmPeriod = lightUpMinutes * 60 * 1000;
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

unsigned long linearBrightnessPwm(unsigned long targetBrightness)
{
    return targetBrightness * targetBrightness / 1023ul;
}
