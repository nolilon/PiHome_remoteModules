#pragma once
#include "weatherstationwireless.h"
#include "wifiConnection.h"

void setup()
{
    connectWiFi();
    WeatherStationWireless station;
    station.getMesurements();
    station.sendMesurements();
    ESP.deepSleep(30e6);
}

void loop()
{}
