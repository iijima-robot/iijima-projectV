#include "cvMain.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "cv_calibration");
  cvMain CM;
  ros::Rate rate(100);
  ROS_INFO("実行開始");
  while(ros::ok()){
    CM.cvMain_Function();
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
