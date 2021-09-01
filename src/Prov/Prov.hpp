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

enum ProvStatus
{
    ST_IDLE,
    ST_WAITING_PROV_DATA,
    ST_PROV_WIFI,
    ST_WIFI_REFUSED,
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
    ComWebServer _webServer;
    IotDevice *_device;
    ProvDataFile _file;

    int _timeout;
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

    Prov(IotDevice &device, String &ssid, int timeout)
    {
        _device = &device;
        _ssid = ssid;
        _timeout = timeout;
        _device->getCom(COM_PORT_WEB_SERVER, &_webServer);
    }

    ~Prov() {}

    void begin(String &fileName)
    {
        if (!LittleFS.begin())
        {
            Serial.println("SPIFFS Mount Failed!");
        }

        _file.setFileName(fileName);

        if (_file.load())
        {
            _status = ST_PROV_WIFI;
            _provOk = true;
        }
        else
        {
            _status = ST_WAITING_PROV_DATA;
        }
    }

    ProvStatus task()
    {
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
                configTime(_file.data.ntpProv.timezone, 0,
                           _file.data.ntpProv.server1.c_str(),
                           _file.data.ntpProv.server2.c_str(),
                           _file.data.ntpProv.server3.c_str());
            }
            else
            {
                _status = ST_WIFI_REFUSED;
            }
            break;
        case ST_WIFI_REFUSED:
            _device->sendData("WIFI_REFUSED", "Prov");
            _status = ST_WAITING_PROV_DATA;
            break;
        case ST_PROV_NTP:
            if (_getYear() > 120)
            {
                _status = ST_PROV_MQTT;
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
                    _device->sendData(WiFi.macAddress(), "Prov");
                }
            }
            _status = ST_WAITING_CONFIRM;
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

    ProvStatus getProvStatus()
    {
        return _status;
    }
};