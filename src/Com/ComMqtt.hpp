/**
 * @file ComMqtt.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <MQTT.h>
#include "Com/Com.h"

#ifdef ESP32
#include <WiFi.h>
#include "SPIFFS.h"
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

class ComMqtt : public MQTTClient, public Com
{
private:
    static String _topic;
    static String _data;
    static bool _dataAvailable;
    
public:
    ComMqtt() {}

    ~ComMqtt() {}

    void init() override
    {
        MQTTClient::onMessage(onData);
    }

    bool connect(String url, WiFiClient &wifiClient){
        int aux = 0;
        MQTTClient::begin(url.c_str(), wifiClient);
        Serial.print("Connecting to ");
        Serial.println(url);
        while (!MQTTClient::connect(WiFi.macAddress().c_str()))
        {
            aux++;
            delay(1000);
            if (aux >= 10)
            {
                Serial.println("Connection timeout!");
                return false;
            }
        }
        if (MQTTClient::connected())
            Serial.println("Connected");
        return MQTTClient::connected();
    }

    void sendData(const String &destiny, const String &data) override
    {
        MQTTClient::publish(destiny, data);
    }

    bool dataAvailable() override
    {
        return _dataAvailable;
    }

    String getData() override
    {
        return _data;
    }
};

String ComMqtt::_topic;
String ComMqtt::_data;
bool ComMqtt::_dataAvailable;