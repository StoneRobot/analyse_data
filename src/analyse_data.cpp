#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32MultiArray.h>
#include <string.h>

#include "json/json.h"

#include "hirop_msgs/ObjectArray.h"
#include "analyse_data/enum_cmd.h"
#include "vector"


class AnalyseData
{
public:
    AnalyseData(ros::NodeHandle n)
    {
        _n = n;
        sub = _n.subscribe("/user_intent", 1, &AnalyseData::subCallback, this);
        pub = _n.advertise<std_msgs::Int32MultiArray>("/action_data", 1);
        Pose_pub = _n.advertise<hirop_msgs::ObjectArray>("object_array", 1);
        isPub = false;
        poses.resize(3);
        float position0[3] ={0.418, -0.65, 0.66};
        float orientation0[4] = {0, 0, -0.706825, 0.707388};
        pushPose(poses[0], position0, orientation0);

        float position1[3] ={0.418, -0.65, 0.38};
        float orientation1[4] = {0, 0, -0.706825, 0.707388};
        pushPose(poses[1], position1, orientation1);

        float position2[3] ={0.85, 0, 0.25};
        float orientation2[4] = {0, 0, 0, 1};
        pushPose(poses[2], position2, orientation2);
    }

private:
    ros::NodeHandle _n;
    ros::Subscriber sub;
    ros::Publisher pub;
    ros::Publisher Pose_pub;
    bool isPub;
    EnumCommand cmd;
    std::vector<hirop_msgs::ObjectArray> poses;

    void subCallback(const std_msgs::StringConstPtr &msg)
    {
        Json::Reader reader;
        Json::Value j_data;
        std::string data = msg->data;
        reader.parse(data, j_data);
        std::string intent = j_data["intent"].asString();
        std::string object = ((j_data["slots"])[0])["normValue"].asString();
        std::string target = ((j_data["slots"])[1])["normValue"].asString();
        if(intent == "begin")
        {
            this->isPub = true; 
            return;
        }
        else if(intent == "stop")
        {
            this->isPub = false;
        }
        if(this->isPub)
        {
            std_msgs::Int32MultiArray enum_command;
            enum_command.data.resize(3);
            enum_command.data[0] = cmd.find_intent(intent);
            enum_command.data[1] = cmd.find_object(object);
            enum_command.data[2] = cmd.find_target(target);
            ROS_INFO_STREAM(enum_command.data[0] << " " << enum_command.data[1] << " " << enum_command.data[2]);
            ROS_INFO_STREAM(poses[enum_command.data[1]]);
            _n.setParam("/call_bridge/intent", enum_command.data[0]);
            _n.setParam("/call_bridge/target", enum_command.data[2]);
            // 从桌子到货架
            if(enum_command.data[0] == 1)
                this->Pose_pub.publish(poses[2]);
            else
                this->Pose_pub.publish(poses[enum_command.data[1]]);
            // this->pub.publish(enum_command);
        }
    }

    void pushPose(hirop_msgs::ObjectArray& pose, float position[], float orientation[])
    {
        pose.objects.resize(1);
        pose.objects[0].pose.header.frame_id = "base_link";
        pose.objects[0].pose.pose.position.x = position[0];
        pose.objects[0].pose.pose.position.y = position[1];
        pose.objects[0].pose.pose.position.z = position[2];
        pose.objects[0].pose.pose.orientation.x = orientation[0];
        pose.objects[0].pose.pose.orientation.y = orientation[1];
        pose.objects[0].pose.pose.orientation.z = orientation[2];
        pose.objects[0].pose.pose.orientation.w = orientation[3];
    }
};




int main(int argc, char *argv[])
{
    ros::init(argc, argv, "analyse_data_bridge");
    ros::NodeHandle nh;
    AnalyseData analyse_data(nh);
    ros::spin();
    return 0;
}