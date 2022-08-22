#include "addressledslight.h"

// #include <WiFiClient.h>
#include <FastLED.h>
#define LED_PIN     4
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

// namespace {
// WiFiClient _client;
// }

AddressLedsLight::AddressLedsLight(char const *ip, unsigned short port, unsigned ledsNum)
    : _ip(ip),
      _port(port),
      _ledsNum(ledsNum),
      _leds( new CHSV[ledsNum] ),
      _rgbLeds( new CRGB[ledsNum] )
{}

void AddressLedsLight::init()
{
    pinMode(LED_PIN, OUTPUT);
    // digitalWrite(LED_PIN, LOW);
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(_rgbLeds, _ledsNum).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(255);
    for (uint8_t i = 0; i < _ledsNum; ++i) 
    {
        _leds[i] = CHSV(static_cast<uint8_t>(i*3), 255, 0);
        _rgbLeds[i] = _leds[i];
    }
    FastLED.show();
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

    // if ( _client.connected() )
    // {
    //     if ( _justConnected )
    //     {
    //         _client.write( _state == On || _state == TurningOn );
    //         _justConnected = false;
    //     }
        
    //     if ( !_client.available() ) return;

    //     // Serial.println("Data received!");
    //     char command = _client.read();
    //     // Serial.printf("Command: %c\n", command);

    //     if ( command == 'U' ) _state = TurningOn;
    //     else if ( command == 'D') _state = TurningOff;
    // }
    // else 
    // {
    //     _client.connect(_ip, _port);
    //     _justConnected = true;
    // }
}

void AddressLedsLight::adjustColors()
{    
    for (unsigned i = 0; i < _ledsNum; ++i)  
    {
        _leds[i].hue += 1;
        _rgbLeds[i] = _leds[i];
    }
    FastLED.show();
    Serial.print(_rgbLeds[0].red);
    Serial.print(' ');
    Serial.print(_rgbLeds[0].green);
    Serial.print(' ');
    Serial.println(_rgbLeds[0].blue);
}


const uint8_t step = 5;
const uint8_t maxBr = 180;

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
    if (allBrightnessMax) {
        _state = On;
        Serial.println("Turned on");
    }
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
