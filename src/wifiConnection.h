#pragma once

#include <ESP8266WiFi.h>
#include "mywifi.h"

void connectWiFi()
{
    Serial.begin(9600);
    Serial.setDebugOutput(true);
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setOutputPower(18);
    delay(1000);

    WiFi.begin(ssid, password);

    while ( WiFi.status() != WL_CONNECTED ) 
    {        
        delay(500);
        // Serial.print('.');
        Serial.print(WiFi.status());
    }
    Serial.println("done");
    Serial.println(WiFi.localIP());
}
