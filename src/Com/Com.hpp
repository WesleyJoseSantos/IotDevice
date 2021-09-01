/**
 * @file Com.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "WString.h"

class Com
{
private:

public:

    Com(){}

    ~Com() {}

    virtual void init(){}

    virtual void sendData(const String &destiny, const String &data){}

    virtual bool dataAvailable(){ return false; }

    virtual String getData(){ return ""; }
};