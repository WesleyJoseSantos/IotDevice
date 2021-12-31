/**
 * @file Prov.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "IotDevice.hpp"
#include "ProvData.hpp"
#include "WiFiHandler/WiFiHandler.h"

#define PROV_FILE "/prov.json"
#define PROV_PAGE "/prov.html"
#define PROV_SSID "Prov AP"
#define PROV_TIMEOUT 30000

enum ProvStatus
{
    ST_IDLE,
    ST_WAITING_PROV_DATA,
    ST_PROV_WIFI,
    ST_PROV_NTP,
    ST_PROV_MQTT,
    ST_WAITING_CONFIRM,
    ST_PROV_COMPLETE,
    ST_ERR
};

class Prov
{
private:
    ProvStatus _status;
    String _ssid;
    ComWebServer *_webServerHandler;
    ComHandler *_device;
    ProvDataFile _file;

    WiFiClient _wifiClient;
    DNSServer _dns;
    AsyncWebServer _webServer;

    int _timeout;
    int _ntpTimeout;
    bool _provOk;

    bool _waitTime(int time)
    {
        static int timer;
        if (timer == 0)
        {
            timer = time;
            return true;
        }
        timer--;
        delay(1000);
        return false;
    }

    int _getYear()
    {
        time_t now = time(nullptr);
        struct tm timeinfo = {0};
        localtime_r(&now, &timeinfo);
        return timeinfo.tm_year;
    }

public:
    Prov() : _webServer(80)
    {
        _ssid = PROV_SSID;
        _timeout = PROV_TIMEOUT;
        _file.setFileName(PROV_FILE);
    }

    ~Prov() {}

    void setSsid(const String &ssid){
        _ssid = ssid;
    }

    void setTimeout(int timeout){
        _timeout = timeout;
    }

    void setFileName(const String &fileName){
        _file.setFileName(fileName);
    }

    void begin(ComHandler &device)
    {
        _device = &device;
        Serial.printf("1:%p\n", _webServerHandler);
        _device->getCom(COM_PORT_WEB_SERVER, &_webServerHandler);

        if(_webServerHandler == nullptr){
            Serial.printf("4:%p\n", _webServerHandler);
            Serial.println("Erro web server null");
            return;
        }

        _webServerHandler->setHomePage(PROV_PAGE);

        if (!LittleFS.begin())
        {
            Serial.println("LittleFS Mount Failed!");
        }

        if (_file.load())
        {
            Serial.println("Prov File finded");
            _status = ST_PROV_WIFI;
            _provOk = true;
        }
        else
        {
            _status = ST_WAITING_PROV_DATA;
            WiFi.softAP(_ssid);
            _dns.start(53, "*", WiFi.softAPIP());
            // _webServer.addHandler(_webServerHandler);
            // _webServer.begin();
            Serial.println("Prov server started");
        }
    }

    ProvStatus task()
    {
        _dns.processNextRequest();
        switch (_status)
        {
        case ST_IDLE:
            //BEGIN
            break;
        case ST_WAITING_PROV_DATA:
        {
            ComPort com = _device->dataAvailable();
            if (com != COM_PORT_NONE)
            {
                _file.fromJson(_device->getData(com));
                _status = ST_PROV_WIFI;
                _device->sendData("PROV_WIFI");
            }
            else
            {
                if (_file.exists())
                {
                    if (_waitTime(_timeout))
                    {
                        _status = ST_PROV_WIFI;
                    }
                }
            }
        }
        break;
        case ST_PROV_WIFI:
            if (WiFiHandler::connect(_file.data.wifiProv.ssid, _file.data.wifiProv.pass))
            {
                _status = ST_PROV_NTP;
                _device->sendData("PROV_NTP");
                configTime(_file.data.ntpProv.timezone, 0,
                           _file.data.ntpProv.server1.c_str(),
                           _file.data.ntpProv.server2.c_str(),
                           _file.data.ntpProv.server3.c_str());
            }
            else
            {
                _status = ST_WAITING_PROV_DATA;
                _device->sendData("WIFI_REFUSED");
            }
            break;
        case ST_PROV_NTP:
            if (_getYear() > 120)
            {
                _status = ST_PROV_MQTT;
                _device->sendData("PROV_MQTT");
            }else{
                _ntpTimeout++;
                delay(1000);
                if(_ntpTimeout >= 20){
                    _ntpTimeout = 0;
                    _status = ST_PROV_MQTT;
                    _device->sendData("NTP_ERR");
                    delay(4000);
                    _device->sendData("PROV_MQTT");
                }
            }
            break;
        case ST_PROV_MQTT:
        {
            ComMqtt *mqtt;
            WiFiClient client;
            if (_device->portAvailable(COM_PORT_MQTT))
            {
                _device->getCom(COM_PORT_MQTT, mqtt);
                if (mqtt->connect(_file.data.mqttProv.url, client))
                {
                    _status = ST_WAITING_CONFIRM;
                    _device->sendData(WiFi.macAddress(), "Prov");
                }
                else
                {
                    _device->sendData("PROV_MQTT_ERR");
                }
            }
        }
        break;
        case ST_WAITING_CONFIRM:
        {
            ComPort com = _device->dataAvailable();
            if (com != COM_PORT_NONE)
            {
                if (_device->getData() == "C")
                {
                    _file.save();
                    _status = ST_PROV_COMPLETE;
                    _device->sendData("PROV_COMPLETE");
                }
            }
        }
        case ST_PROV_COMPLETE:
            //END
            break;
        default:
            break;
        }

        return _status;
    }

    ProvStatus getStatus()
    {
        return _status;
    }
};