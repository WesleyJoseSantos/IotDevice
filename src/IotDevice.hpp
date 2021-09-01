/**
 * @file IotDevice.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Com/Com.h"
#include <DNSServer.h>
#include "ESPAsyncWebServer.h"
#include "WebServerHandle/WebServerHandle.h"

#ifdef ESP32
#include <AsyncTCP.h>
#include "SPIFFS.h"
#elif defined(ESP8266)
#include <ESPAsyncTCP.h>
#endif

class IotDevice : public ComHandler
{
private:

public:
    IotDevice(){}

    ~IotDevice() {}
};