#include <ESP8266WiFi.h>
#include "mywifi.h"

void connectWiFi()
{
    WiFi.mode(WIFI_OFF);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
    WiFi.begin(ssid, password);

    while ( WiFi.status() != WL_CONNECTED ) delay(1000);
}

#define LIGHT_ALARM

#ifdef LIGHT_ALARM
#include "lightalarm.h"
LightAlarm alarm(6001, 2);

void setup() 
{
    Serial.begin(115200);
    pinMode(2, OUTPUT);
    digitalWrite(2, 0);

    connectWiFi();
}

void loop() 
{
    alarm.loop();
}
#endif