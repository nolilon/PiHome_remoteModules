#include "weatherstation.h"

#include <WiFiClient.h>
WiFiClient client;

#include <Adafruit_BME280.h>
Adafruit_BME280 bme;

WeatherStation::WeatherStation(char const *ip, unsigned short port, unsigned char address, unsigned short powerPin)
    : _ip(ip),
      _port(port),
      _address(address),
      _powerPin(powerPin)
{}

void WeatherStation::init()
{
    pinMode(2, OUTPUT);
    digitalWrite(_powerPin, HIGH);
    bme.begin(_address);
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                    Adafruit_BME280::SAMPLING_X16,
                    Adafruit_BME280::SAMPLING_X16,
                    Adafruit_BME280::SAMPLING_X16,
                    Adafruit_BME280::FILTER_OFF,
                    Adafruit_BME280::STANDBY_MS_1000);
}

void WeatherStation::loop()
{
    auto currentTime = millis();
    if (currentTime - _lastTime < _period) return;
    _lastTime = currentTime;

    // unsigned long start, stop;
    // start = millis();

    if ( client.connected() )
    {
        // Serial.println("Client connected");
        
        char data[4];
        short *tempch = reinterpret_cast<short*>(data);
        short *humidch = reinterpret_cast<short*>(data+2);
        *tempch = bme.readTemperature()*10 - 5;
        *humidch = bme.readHumidity()*10;
        // Serial.printf("T: %f, H: %f\n", (*tempch)/10., (*humidch)/10.);

        client.write(data,4);
        // Serial.println("Data sent");

        // if ( client.available() ) 
        // {
            // Serial.println("Data received!");
            // char command = client.read();
            // Serial.printf("Command: %c\n", command);

            // if ( command == 'Q' ) client.write('A');
        // }
    }
    else client.connect(_ip, _port);
    
    // stop = millis();
    // Serial.printf("Bme loop takes %lu ms\n\n", stop-start);
}
