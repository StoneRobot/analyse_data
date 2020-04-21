#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32MultiArray.h>
#include <string.h>
#include <vector>
#include <moveit/move_group_interface/move_group_interface.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <moveit_msgs/MoveItErrorCodes.h>
#include <stdlib.h>
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
        ROS_INFO_STREAM("Object: " << Object);
    }
}

void angle2rad(std::vector<double>& angle)
{
    int i = 0;
    for(i; i < angle.size(); ++i)
    {
        angle[i] = (angle[i]/180)*3.1415926;
        std::cout << angle[i] << " ";
    }
    std::cout << std::endl;
}

void addCollision(bool  b)
{
    if(b == true)
    {
        system("rosservice call /clear_octomap");
        system("rosservice call /clean_pcl");
        system("rosservice call /look");
        // system("rosservice call /add_collision");
        ros::WallDuration(1.0).sleep();
    }
}

void go(const std::vector<double>& Value, moveit::planning_interface::MoveGroupInterface& move_group)
{
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    move_group.setJointValueTarget(Value);
    bool success;
    for(int i=0; i<5; ++i)
    {
        success = (move_group.plan(my_plan) == moveit_msgs::MoveItErrorCodes::SUCCESS);
        if(success)
            break;
    }
    moveit_msgs::MoveItErrorCodes codes;
    if(success)
    do
    {
        codes = move_group.move();
    }
    while (codes.val !=  moveit_msgs::MoveItErrorCodes::SUCCESS);
}


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "analyse_data_bridge");
    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::Subscriber sub = nh.subscribe("/user_intent", 1, subCallback);
    ros::ServiceClient detection_client = nh.serviceClient<hirop_msgs::detection>("detection");
    ros::Publisher Object_pub = nh.advertise<hirop_msgs::ObjectArray>("object_array", 1);
    moveit::planning_interface::MoveGroupInterface move_group("arm");
    
    std::vector<double> joint_group_positions1={4.2982, -96.465, 174.814, -69.8859, 92.1213, -20.163};
    std::vector<double> joint_group_positions2={4.296766, -93.700937, 205.049758, -71.801747, 81.01799, 11.016716};
    angle2rad(joint_group_positions1);
    angle2rad(joint_group_positions2);
    
    
    move_group.setNamedTarget("home");
    move_group.move();
    
    bool add_collision = true;

    while (ros::ok())
    {
        // ros::spinOnce();
        // ROS_INFO("spin once");
        if(IsAction == false)
        {
            IsAction = false;
            // test
            ROS_INFO("Press 'enter' to continue");
            std::cin.ignore();
            nh.getParam("object", Object);
            
            nh.getParam("add_collision", add_collision);
            addCollision(add_collision);
            go(joint_group_positions1, move_group);
            bool isUseDetection = true;
            nh.getParam("is_use_detection", isUseDetection);
            // 调试使用
            if(isUseDetection)
            {
                ROS_INFO("call detection");
                ROS_INFO("UseDetection");
                hirop_msgs::detection det_srv;
                det_srv.request.objectName = Object;
                det_srv.request.detectorName = "Yolo6d";
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
                    ROS_INFO("to shelf top");
                    Object_pub.publish(cmd.getObjectArray(0));
                }
                else if(Object == "milk")
                {
                    ROS_INFO("to shelf end");
                     Object_pub.publish(cmd.getObjectArray(1));
                }
            }
        }
    }
    return 0;
}
