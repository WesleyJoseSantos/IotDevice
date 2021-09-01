/**
 * @file ComSerial.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Stream.h"
#include "Com/Com.h"

class ComSerial : public Com
{
private:
    Stream *_stream;
public:
    ComSerial(Stream &stream) {
        _stream = &stream;
    }

    ~ComSerial() {}

    void init() override {
        _stream->setTimeout(250);
    }

    void sendData(const String &destiny, const String &data) override {
        _stream->println(data);
    }

    bool dataAvailable() override {
        return _stream->available();
    }

    String getData() override {
        return _stream->readStringUntil('\n');
    }
};