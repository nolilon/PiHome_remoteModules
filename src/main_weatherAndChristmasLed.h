#pragma once
#include "wifiConnection.h"
#include "weatherstation.h"
#include "addressledslight.h"


void setup() 
{
    Serial.begin(115200);

    WeatherStation weather(notebookbIp, 6000, 0x76, 2);
    weather.init();

    AddressLedsLight light(notebookbIp, 6002, 30);
    light.init();

    connectWiFi();

    while (true)
    {
        light.loop();
        weather.loop();
        yield();
    }
    
}

void loop() 
{
    // weather.loop();
    // light.loop();
}