#include <ESP8266WiFi.h>
#include "mywifi.h"

void connectWiFi();

// #define WEATHER_AND_CHRISTMAS_LED
// #define WEATHER
#define LIGHT_ALARM


#ifdef LIGHT_ALARM
#include "lightalarm.h"
LightAlarm alarm(piIp, 6001, 4);

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
#endif


#ifdef WEATHER
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
#endif


#ifdef WEATHER_AND_CHRISTMAS_LED
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
#endif


void connectWiFi()
{
    // WiFi.mode(WIFI_OFF);
    // delay(1000);
    // WiFi.mode(WIFI_STA);
    Serial.println();
    delay(2000);

    if (WiFi.SSID() != ssid) WiFi.begin(ssid, password);
    else Serial.println("WiFi config loaded");

    Serial.printf("Connecting to %s", ssid);

    while ( WiFi.status() != WL_CONNECTED ) 
    {        
        static int secs = 0;
        ++secs;
        delay(1000);
        Serial.print('.');
        if (secs == 60) 
        {
            Serial.print("reload");
            WiFi.begin(ssid, password);
        }
    }
    Serial.println("done");
    Serial.println(WiFi.localIP());
}
