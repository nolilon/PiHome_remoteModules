#pragma once
#include "wifiConnection.h"
#include "lightalarm.h"


LightAlarm alarm(piIp, 56701, 4);

void setup() 
{
    Serial.begin(115200);
    alarm.init();
    connectWiFi();
}

void loop() 
{
    alarm.loop();
}