#pragma once

struct CHSV;
struct CRGB;

class AddressLedsLight
{
public:
    AddressLedsLight(char const *ip, unsigned short port, unsigned ledsNum);
    
    void init();
    void loop();

private:
    char const *const _ip;
    unsigned short const _port;
    // unsigned short const _ledsPin; because it is template patameter of FastLED library
    unsigned const _ledsNum;

    CHSV *const _leds;
    CRGB *const _rgbLeds;

    unsigned long _lastTime = 0;

    enum State {TurningOn, On, TurningOff, Off};
    State _state = TurningOn;

    bool _justConnected = true;

    void adjustColors();
    void brightnessUp();
    void brightnessDown();
};