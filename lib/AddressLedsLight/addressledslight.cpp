#include "addressledslight.h"

#include <FastLED.h>
#define LED_PIN     0
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B


AddressLedsLight::AddressLedsLight(char const *ip, unsigned short port, unsigned ledsNum)
    : _ip(ip),
      _port(port),
    //   _ledsPin(ledPin),
      _ledsNum(ledsNum),
      _leds( new CHSV[ledsNum] ),
      _rgbLeds( new CRGB[ledsNum] )
{}

void AddressLedsLight::init()
{
    delay(2000);
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(_rgbLeds, _ledsNum).setCorrection( TypicalLEDStrip );
    for (uint8_t i = 0; i < _ledsNum; ++i) _leds[i] = {static_cast<uint8_t>(i*3), 255, 0};
}

void AddressLedsLight::loop()
{
    auto currentTime = millis();
    if ( currentTime - _lastTime < 30 ) return;
    _lastTime = currentTime;

    switch (_state)
    {
    case TurningOn:
        brightnessUp();
        adjustColors();
        break;
    
    case On:
        adjustColors();
        break;

    case TurningOff:
        brightnessDown();
        adjustColors();
        break;

    case Off:
        break;
    }

    if ( _client.connected() )
    {
        if ( _justConnected )
        {
            _client.write( _state == On || _state == TurningOn );
            _justConnected = false;
        }
        
        if ( !_client.available() ) return;

        // Serial.println("Data received!");
        char command = _client.read();
        // Serial.printf("Command: %c\n", command);

        if ( command == 'U' ) _state = TurningOn;
        else if ( command == 'D') _state = TurningOff;
    }
    else 
    {
        _client.connect(_ip, _port);
        _justConnected = true;
    }
}

void AddressLedsLight::adjustColors()
{    
    for (unsigned i = 0; i < _ledsNum; ++i)  
    {
        _leds[i].hue += 1;
        _rgbLeds[i] = _leds[i];
    }
    FastLED.show();
}


const uint8_t step = 5;
const uint8_t maxBr = 120;

void AddressLedsLight::brightnessUp()
{    
    bool allBrightnessMax = true;
    for (unsigned i = 0; i < _ledsNum; ++i)
    {
        if ( _leds[i].value < maxBr - step ) 
        {
            _leds[i].value += step;
            allBrightnessMax = false;
        }
        else if ( _leds[i].value < maxBr) _leds[i].value = maxBr;
    }
    if (allBrightnessMax) _state = On;
}

void AddressLedsLight::brightnessDown()
{    
    bool allBrightnessOff = true;
    for (unsigned i = 0; i < _ledsNum; ++i)
    {
        if (_leds[i].value > step)
        {
            _leds[i].value -= step;
            allBrightnessOff = false;
        }
        else if ( _leds[i].value > 0 ) _leds[i].value = 0;
    }
    if (allBrightnessOff) _state = Off;
}
