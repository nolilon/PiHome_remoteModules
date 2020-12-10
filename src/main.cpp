#include <ESP8266WiFi.h>
#include "mywifi.h"


void setup() 
{
    WiFi.mode(WIFI_OFF);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
    WiFi.begin(ssid, password);

    while ( WiFi.status() != WL_CONNECTED ) delay(1000);

    while (true) loop();
}

void loop() 
{

}
