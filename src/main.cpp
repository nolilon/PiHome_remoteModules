#include <ESP8266WiFi.h>
#include "mywifi.h"

void connectWiFi();

#define WEATHER_AND_CHRISTMAS_LED
// #define LIGHT_ALARM


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


#ifdef WEATHER_AND_CHRISTMAS_LED
#include "weatherstation.h"
WeatherStation weather(notebookbIp, 6000, 0x76, 2);

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
#endif


void connectWiFi()
{
    Serial.println();
    delay(2000);

    if (WiFi.SSID() != ssid) WiFi.begin(ssid, password);
    else Serial.println("WiFi config loaded");

    Serial.printf("Connecting to %s", ssid);

    while ( WiFi.status() != WL_CONNECTED ) 
    {        
        delay(1000);
        Serial.print('.');
    }
    Serial.println("done");
}
