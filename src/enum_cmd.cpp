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