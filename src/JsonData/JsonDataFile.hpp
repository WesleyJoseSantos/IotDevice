/**
 * @file JsonDataFile.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "IJsonData.hpp"

#ifdef  ESP8266
#include "LittleFS.h"
#else
#include "LittleFS.h"
#define LittleFS LittleFS
#endif  //ESP8266

/**
 * @brief Representa um arquivo capaz de salvar/recuperar seu conteúdo usando strings no formato json
 * 
 */
class JsonDataFile : public IJsonData
{
private:
    String _fileName;

public:
    JsonDataFile() {}
    ~JsonDataFile() {}

    /**
     * @brief Testa se o arquivo existe
     * 
     * @return true se existe
     * @return false se não existe
     */
    bool exists(){
        return LittleFS.exists(_fileName);
    }

    /**
     * @brief Salva o conteúdo desse objeto no arquivo que ele representa
     * 
     */
    void save(){
        String jsonStr = toJson();
        File file = LittleFS.open(_fileName, "w+");
        file.print(jsonStr);
        file.close();
    }

    /**
     * @brief Carrega o conteúdo do arquivo
     * 
     * @return true 
     * @return false 
     */
    bool load(){
        File file = LittleFS.open(_fileName, "r");
        if(file){
            String jsonStr = file.readString();
            fromJson(jsonStr);
            file.close();
            return true;
        }
        return false;
    }

    /**
     * @brief Remove o arquivo representado
     * 
     */
    void remove(){
        LittleFS.remove(_fileName);
    }

    /**
     * @brief Define o nome do arquivo
     * 
     * @param fileName 
     */
    void setFileName(const String &fileName){
        _fileName = fileName;
    }

    /**
     * @brief Obtêm o nome do arquivo
     * 
     * @return String 
     */
    String getFileName(){
        return _fileName;
    }
};