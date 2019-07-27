#ifndef MAIN_H
#define MAIN_H

#include "ClassDefinition.h"

class cvMain{
  private:
  int rgb;
  std::string OPENCV_WINDOW;
  std::string Camera_WINDOW;
  std::string CV_WINDOW;
  cv::Mat output_image;//出力画像
public:
  PostMan pm_;
  std::vector<cv::Mat> chess_object;
  cvMain();
  ~cvMain();

  int cvMain_Function();
  //void imageCb(const sensor_msgs::ImageConstPtr& msg);
};

#endif