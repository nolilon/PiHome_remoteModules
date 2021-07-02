#pragma once
#include "wifiConnection.h"
#include "lightalarm.h"


LightAlarm alarm(piIp, 56701, 4);

void setup() 
{
    alarm.init();
    connectWiFi();
}

void loop() 
{
    alarm.loop();
}