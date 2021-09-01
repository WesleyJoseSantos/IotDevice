/**
 * @file WiFiHandler.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

class WiFiHandler
{
private:
    
public:
    WiFiHandler() {}
    ~WiFiHandler() {}

    static bool connect(String ssid, String pass){
        int aux = 0;
        WiFi.begin(ssid, pass);
        Serial.print("Connecting to ");
        Serial.print(ssid);
        Serial.println(" ...");
        while (WiFi.status() != WL_CONNECTED)
        {
            aux++;
            delay(1000);
            if (aux >= 25)
            {
                Serial.println("Connection timeout!");
                return false;
            }
        }
        if (WiFi.isConnected())
            Serial.println("Connected");
        return WiFi.isConnected();
    }
};