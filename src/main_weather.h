#pragma once
#include "wifiConnection.h"
#include "weatherstation.h"


WeatherStation weather(piIp, 16000, 0x76, 2);

void setup() 
{
    Serial.begin(115200);
    weather.init();
    connectWiFi();    
}

void loop() 
{
    weather.loop();
}
