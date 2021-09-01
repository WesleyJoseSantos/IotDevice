/**
 * @file ProvFile.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "JsonData/JsonData.h"
#include "Arduino.h"

enum ProvItem
{
    PROV_WIFI,
    PROV_NTP,
    PROV_MQTT
};

struct WifiProvData{
    String ssid;
    String pass;
};

struct NtpProvData
{
    String server1;
    String server2;
    String server3;
    int timezone;
};

struct MqttProvData{
    String url;
    int port;
    String user;
    String pass;
};

class ProvData : public IJsonData
{
private:

public:
    int provItems;
    NtpProvData ntpProv;
    WifiProvData wifiProv;
    MqttProvData mqttProv;

    ProvData() {}
    ~ProvData() {}

    void enableProvItem(ProvItem item){
        provItems = ((provItems) |= (1UL << (item)));
    }

    void disableProvItem(ProvItem item){
        provItems = ((provItems) &= ~(1UL << (item)));
    }

    void setProvItems(int provItems){
        provItems = provItems;
    }

    bool isEnabled(ProvItem item){
        return (((provItems) >> (item)) & 0x01);
    }

    void fromJson(const String &jsonStr) override {
        StaticJsonDocument<1023> jsonDoc;
        deserializeJson(jsonDoc, jsonStr);

        provItems = jsonDoc["provItems"];

        if(jsonDoc.containsKey("ntpProv")){
            ntpProv.server1 = jsonDoc["ntpProv"]["server1"].as<String>();
            ntpProv.server2 = jsonDoc["ntpProv"]["server2"].as<String>();
            ntpProv.server3 = jsonDoc["ntpProv"]["server3"].as<String>();
            ntpProv.timezone = jsonDoc["ntpProv"]["timezone"];
        }

        if(jsonDoc.containsKey("wifiProv")){
            wifiProv.ssid = jsonDoc["wifiProv"]["ssid"].as<String>();
            wifiProv.pass = jsonDoc["wifiProv"]["pass"].as<String>();
        }

        if(jsonDoc.containsKey("mqttProv")){
            mqttProv.url = jsonDoc["mqttProv"]["url"].as<String>(); 
            mqttProv.port = jsonDoc["mqttProv"]["port"]; 
            mqttProv.user = jsonDoc["mqttProv"]["user"].as<String>();
            mqttProv.pass = jsonDoc["mqttProv"]["pass"].as<String>();
        }
    }

    String toJson() override {
        StaticJsonDocument<1023> jsonDoc;
        String jsonStr;

        jsonDoc["provItems"] = provItems;

        jsonDoc["ntpProv"]["server1"] = ntpProv.server1;
        jsonDoc["ntpProv"]["server2"] = ntpProv.server2;
        jsonDoc["ntpProv"]["server3"] = ntpProv.server3;
        jsonDoc["ntpProv"]["timezone"] = ntpProv.timezone;

        jsonDoc["wifiProv"]["ssid"] = wifiProv.ssid;
        jsonDoc["wifiProv"]["pass"] = wifiProv.pass;

        jsonDoc["mqttProv"]["url"] = mqttProv.url;
        jsonDoc["mqttProv"]["port"] = mqttProv.port;
        jsonDoc["mqttProv"]["user"] = mqttProv.user;
        jsonDoc["mqttProv"]["pass"] = mqttProv.pass;

        serializeJson(jsonDoc, jsonStr);
        return jsonStr;    
    }
};

class ProvDataFile : public JsonDataFile
{
private:
        
public:
    ProvData data;

    ProvDataFile() {}
    ~ProvDataFile() {}

    void fromJson(const String &jsonStr) override {
        data.fromJson(jsonStr);
    }

    String toJson() override {
        return data.toJson();
    }
};