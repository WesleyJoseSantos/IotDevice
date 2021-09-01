/**
 * @file Com.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Com\Com.h"

enum ComPort{
    COM_PORT_NONE=-1,
    COM_PORT_SERIAL,
    COM_PORT_MQTT,
    COM_PORT_WEB_SERVER,
    COM_PORT_COUNT,
};

class ComHandler
{
private:
    Com *coms[COM_PORT_COUNT];

public:

    ComHandler(){
        for (size_t i = 0; i < COM_PORT_COUNT; i++)
        {
            coms[i] = nullptr;
        }
    }

    ~ComHandler() {}

    void attachCom(Com &com, ComPort comPort){
        coms[comPort] = &com;
    }

    void dettachCom(ComPort comPort){
        coms[comPort] = nullptr;
    }

    bool portAvailable(ComPort comPort){
        if(comPort >= 0 && comPort < COM_PORT_COUNT){
            return coms[comPort] != nullptr;
        }
        return false;
    }

    void sendData(const String &destiny, const String &data){
        for (size_t i = 0; i < COM_PORT_COUNT; i++)
        {
            if(portAvailable((ComPort)i)){
                coms[i]->sendData(data, destiny);
            }
        }
        
    }

    void sendData(ComPort comPort, const String &destiny, const String &data){
        if(portAvailable(comPort)){
            coms[comPort]->sendData(data, destiny);
        }
    }

    ComPort dataAvailable(){
        for (size_t i = 0; i < COM_PORT_COUNT; i++)
        {
            if(portAvailable((ComPort)i)){
                if (coms[i]->dataAvailable()){
                    return ((ComPort)i);
                }
            }
        }
        return COM_PORT_NONE;
    }

    String getData(ComPort comPort){
        if(portAvailable(comPort)){
            return coms[comPort]->getData();
        }else{
            return "";
        }
    }

    String getData(){
        for (size_t i = 0; i < COM_PORT_COUNT; i++)
        {
            if(portAvailable((ComPort)i)){
                if (coms[i]->dataAvailable()){
                    return coms[i]->getData();
                }
            }
        }
        return "";
    }

    void getCom(ComPort comPort, Com *com){
        com = coms[comPort];
    }
};