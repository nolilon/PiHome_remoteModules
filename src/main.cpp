// #define WEATHER_AND_CHRISTMAS_LED

#include "main_lightAlarm.h"


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

