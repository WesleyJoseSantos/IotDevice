/**
 * @file IJsonData.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "WString.h"

/**
 * @brief Representa uma estrutura de dados capaz de manipular seu conteúdo usando strings no formato json
 * 
 */
class IJsonData
{
private:
    
public:
    IJsonData() {}
    ~IJsonData() {}

    /**
     * @brief Recupera o conteúdo de uma string no formato json
     * 
     * @param jsonStr 
     */
    virtual void fromJson(const String &jsonStr) = 0;
 
    /**
     * @brief Retorna uma string no formato json contendo as informações da instância
     * 
     * @return String 
     */
    virtual String toJson() = 0;
};