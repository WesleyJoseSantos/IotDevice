/**
 * @file ComWebServer.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Com/Com.h"
#include "WebServerHandle/WebServerHandle.hpp"

class ComWebServer : public WebServerHandler, public Com
{
private:
    String _dataToSend;
    String _dataToReceive;
    bool _dataSended;
    bool _dataReceived;
public:
    ComWebServer() {}

    ~ComWebServer() {}

    void init() override {
        
    }

    void handleRequest(AsyncWebServerRequest *request) override {
        
        if(request->method() == 1){
            if(request->hasParam("data")){
                String type = getContentType(request->url());
                request->send(200, type, _dataToSend);
                _dataSended = true;
                return;
            }
        }
        
        if(request->method() == 2){
            if(request->hasParam("data")){
                _dataToReceive = request->getParam("data")->value();
                _dataReceived = true;
                request->send(200, "text/html", "OK");
                return;
            }
        }

        WebServerHandler::handleRequest(request);
    }

    void sendData(const String &destiny, const String &data) override {
        _dataToSend = data;
    }

    bool dataAvailable() override {
        return _dataReceived;
    }

    String getData() override {
        return _dataToReceive;
    }
};