#include "analyse_data/enum_cmd.h"

// EnumCommand::EnumCommand()
// {
//     Intent["place"] = Json::Value(intent::place);
//     Intent["replace"] = Json::Value(intent::replace);
//     // Intent["begin"] = Json::Value(intent::begin);
//     // Intent["stop"] = Json::Value(intent::stop);

//     Object["牛奶盒"] = Json::Value(object::milk);
//     Object["可乐罐"] = Json::Value(object::coke);

//     Target["架子"] = Json::Value(target::shelf);
//     Target["架子第一层"] = Json::Value(target::shelf_first);
//     Target["桌子"] = Json::Value(target::table);
// }

// int EnumCommand::find_intent(std::string str)
// {
//     return Intent[str].asInt();
// }
// int EnumCommand::find_object(std::string str)
// {
//     return Object[str].asInt();
// }
// int EnumCommand::find_target(std::string str)
// {
//     return Target[str].asInt();
// }

EnumCommand::EnumCommand()
{
    Intent["place"] = Json::Value("place");
    Intent["replace"] = Json::Value("replace");
    // Intent["begin"] = Json::Value(intent::begin);
    // Intent["stop"] = Json::Value(intent::stop);

    Object["牛奶盒"] = Json::Value("milk");
    Object["可乐罐"] = Json::Value("coke");

    Target["架子"] = Json::Value("shelf");
    Target["架子第一层"] = Json::Value("shelf_first");
    Target["桌子"] = Json::Value("table");
    hirop_msgs::ObjectArray pose;
    // initPose(pose, 0.418, -0.65, 0.66, 0, 0, -0.706825, 0.707388);
    initPose(pose, 0.418, -0.80, 0.66, 0, 0, -0.706825, 0.707388);
    poses.push_back(pose);
    // initPose(pose, 0.418, -0.65, 0.38, 0, 0, -0.706825, 0.707388);
    initPose(pose, 0.418, -0.80, 0.38, 0, 0, -0.706825, 0.707388);
    poses.push_back(pose);
}

void EnumCommand::initPose(hirop_msgs::ObjectArray& pose, float x, float y, float z, float qx, float qy, float qz, float qw)
{
    pose.objects.resize(1);
    pose.objects[0].pose.header.frame_id = "base_link";
    pose.objects[0].pose.pose.position.x = x;
    pose.objects[0].pose.pose.position.y = y;
    pose.objects[0].pose.pose.position.z = z;
    pose.objects[0].pose.pose.orientation.x = qx;
    pose.objects[0].pose.pose.orientation.y = qy;
    pose.objects[0].pose.pose.orientation.z = qz;
    pose.objects[0].pose.pose.orientation.w = qw;
}

std::string EnumCommand::find_intent(std::string str)
{
    return Intent[str].asString();
}
std::string EnumCommand::find_object(std::string str)
{
    return Object[str].asString();
}
std::string EnumCommand::find_target(std::string str)
{
    return Target[str].asString();
}