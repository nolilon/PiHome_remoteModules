#include "lightalarm.h"

#include "Arduino.h"
#include <WiFiClient.h>

WiFiClient _client;

LightAlarm::LightAlarm(unsigned short port, unsigned char pwmPin)
    : _port(port),
      _pinPwm(pwmPin)
{}

void LightAlarm::loop()
{
    if ( _inProgress ) 
        checkLight();
    else 
        checkCommand();
}

void LightAlarm::checkCommand()
{
    auto currentTime = millis();
    if ( currentTime - _lastTime < 5000 ) return;
    _lastTime = currentTime;

    Serial.println("Checking command...");

    if ( _client.connected() )
    {
        Serial.println("Client connected");
        if ( !_client.available() ) return;

        Serial.println("Data received!");
        char command = _client.read();
        Serial.printf("Command: %c\n", command);

        if ( command == 'S' ) _inProgress = true;
        // else if (command == 'F') analogWrite(_pinPwm, 0);
    }
    else _client.connect("192.168.1.68", _port);
}

void LightAlarm::checkLight()
{
    constexpr unsigned long lightUpDuration = 900000;
    constexpr unsigned long period = lightUpDuration / 1024;

    auto currentTime = millis();
    if ( currentTime - _lastTime < period ) return;
    _lastTime = currentTime;

    if (_currentPwm < 1023) ++_currentPwm;

    Serial.println(_currentPwm);
    analogWrite(_pinPwm, _currentPwm);

    if (_currentPwm == 1023) 
    {
        _currentPwm = 0;
        _inProgress = false;
    }
}
