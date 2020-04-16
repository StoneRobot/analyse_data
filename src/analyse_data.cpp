#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32MultiArray.h>
#include <string.h>
#include <vector>
#include <moveit/move_group_interface/move_group_interface.h>

#include "analyse_data/enum_cmd.h"

#include "hirop_msgs/detection.h"

std::string Intent;
std::string Object;
std::string Target;
bool IsAction = false;
EnumCommand cmd;

void subCallback(const std_msgs::String::ConstPtr& msg)
{
    static bool isPub = false;
    Json::Reader reader;
    Json::Value j_data;
    std::string data = msg->data;
    reader.parse(data, j_data);
    std::string intent = j_data["intent"].asString();
    std::string object = ((j_data["slots"])[0])["normValue"].asString();
    std::string target = ((j_data["slots"])[1])["normValue"].asString();

    if(intent == "begin")
    {
        isPub = true; 
        return;
    }
    else if(intent == "stop")
    {
        isPub = false;
        return;
    }
    else if(isPub)
    {
        IsAction = true;
        Intent = cmd.find_intent(intent);
        Object = cmd.find_object(object);
        Target = cmd.find_target(target);
    }
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "analyse_data_bridge");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/user_intent", 1, subCallback);
    ros::ServiceClient detection_client = nh.serviceClient<hirop_msgs::detection>("detection");
    ros::Publisher Object_pub = nh.advertise<hirop_msgs::ObjectArray>("object_array", 1);
    moveit::planning_interface::MoveGroupInterface move_group("arm");
    std::vector<double> joint_group_positions={8.120842, -86.482752, 152.566045, -72.321156, 103.847033, -32.369842};
    
    while (ros::ok())
    {
        ros::spinOnce();
        if(IsAction == true)
        {
            IsAction = false;
            bool isUseDetection = true;
            move_group.setJointValueTarget(joint_group_positions);
            move_group.move();
            nh.getParam("is_use_detection", isUseDetection);
            // 调试使用
            if(isUseDetection)
            {
                hirop_msgs::detection det_srv;
                det_srv.request.objectName = Object;
                det_srv.request.detectorName = "";
                det_srv.request.detectorType = 1;
                det_srv.request.detectorConfig = "";
                if(detection_client.call(det_srv))
                {
                    ROS_INFO_STREAM("detection result is " << det_srv.response.result);
                }
                else
                {
                    ROS_INFO("check detection service!!");
                }
            }
            else
            {
                if(Object == "coke")
                {
                    Object_pub.publish(cmd.getObjectArray(0));
                }
                else if(Object == "milk")
                {
                     Object_pub.publish(cmd.getObjectArray(1));
                }
            }
        }
    }
    return 0;
}