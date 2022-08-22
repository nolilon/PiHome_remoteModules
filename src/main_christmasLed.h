#pragma once
// #include "wifiConnection.h"
#include "Arduino.h"
// #include "weatherstation.h"
#include "addressledslight.h"

// AddressLedsLight light(piIp, 56702, 30);
AddressLedsLight light("", 56702, 120);

void setup() 
{
    Serial.begin(9600);
    // connectWiFi();   
    light.init();
}

void loop() 
{
    light.loop();
}