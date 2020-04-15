#pragma once
#include "json/json.h"
#include <map>
#include <string.h>

class EnumCommand
{
public:
    EnumCommand();
    // int find_intent(std::string str);
    // int find_object(std::string str);
    // int find_target(std::string str); 
    std::string find_intent(std::string);
    std::string find_object(std::string);
    std::string find_target(std::string);

private:
    enum intent
    {
        replace,
        place
    };
    enum object
    {
        coke,
        milk
    };
    enum target
    {
        
        shelf,
        shelf_first,
        table
    };
    Json::Value Intent; 
    Json::Value Object;
    Json::Value Target; 
};



    









