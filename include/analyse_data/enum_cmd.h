#pragma once
#include "json/json.h"
#include <map>
#include <string.h>
#include "hirop_msgs/ObjectArray.h"

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
    void initPose(hirop_msgs::ObjectArray& pose, float x, float y, float z, float qx, float qy, float qz, float qw);
    hirop_msgs::ObjectArray getObjectArray(int i)
    {
        return poses[i];
    }


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
    std::vector<hirop_msgs::ObjectArray> poses;
};



    









