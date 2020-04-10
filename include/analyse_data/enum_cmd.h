#pragma once
#include "json/json.h"
#include <map>
#include <string.h>

class EnumCommand
{
public:
    EnumCommand();
    // virtual ~EnumCommand();
    int find_intent(std::string str);
    int find_object(std::string str);
    int find_target(std::string str); 

private:
    enum intent
    {
        replace,
        place,
        // begin,
        // stop
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
        table,
    };
    // typedef std::multimap<std::string, intent> Intent_map;
    // std::multimap<std::string, intent> Intent;
    // Intent.insert(std::multimap<std::string, intent>::value_type("place", intent::place));

    //{"place" : intent::place, "replace" : intent::replace, "begin" : intent::begin, "stop" : intent::stop};
    Json::Value Intent; 
    // {"牛奶盒" : object::milk, "可乐罐" : object::coke};
    Json::Value Object;
    //{"架子" : target::shelf, "桌子" : target::table};
    Json::Value Target; 
};



    









