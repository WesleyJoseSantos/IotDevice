/**
 * @file WebServerHandle.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "ESPAsyncWebServer.h"
#ifdef  ESP8266
#include "FS.h"
#else
#include "SPIFFS.h"
#define SPIFFS SPIFFS
#endif  //ESP8266

class WebServerHandler : public AsyncWebHandler
{
protected:
    String homepage;

    String getContentType(String url){
        if(url.endsWith("json")){
            return "application/json";
        }else if(url.endsWith("css")){
            return "text/css";
        }else if(url.endsWith("png")){
            return "image/png";
        }else if(url.endsWith("ico")){
            return "image/x-icon";
        }else if(url.endsWith("js")){
            return "application/javascript";
        }else{
            return "text/html";
        }
    }

public:
    WebServerHandler() {
        homepage = "/home.html";
    }
    
    virtual ~WebServerHandler() {}

    bool canHandle(AsyncWebServerRequest *request){
        return true;
    }

     virtual void handleRequest(AsyncWebServerRequest *request){
        if(request->method() == 1){
            if(request->url() == "/"){
                if(SPIFFS.exists(homepage)){
                    request->send(SPIFFS, homepage, "text/html");
                }else{
                    request->send(404, "text/html", "file not exists");
                    Serial.println(homepage);
                }
            }else{
                if(SPIFFS.exists(request->url())){
                    String type = getContentType(request->url());
                    request->send(SPIFFS, request->url(), type);
                }else{
                    Serial.println(request->url());
                    request->send(404, "text/html", "file not exists");
                }
            }
        }
    }

    void setHomePage(const String &homepage){
        this->homepage = homepage;
    }
};