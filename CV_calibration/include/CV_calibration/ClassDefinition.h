#ifndef ClassDefinition_H
#define ClassDefinition_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <librealsense2/rs.h>

///////////////////////////////////////
//構造体
///////////////////////////////////////
//カメラ情報
struct Camera_INFO{
    int width;
    int height;
};

struct cameraData{
    std::string camera_name;
    Camera_INFO RGB_INFO;
    Camera_INFO Depth_INFO;
    cv::Mat RGB_Gazou;
    cv::Mat Depth_Gazou;
};

class postData{
    private:
    image_transport::Subscriber image_sub_;
    image_transport::Subscriber depth_image;
    ros::Subscriber rgb_info;
    ros::Subscriber depth_info;
    public:
    cameraData camera;
    void getData(ros::NodeHandle nh,image_transport::ImageTransport it);
    void ros_RGB_TOcv_Converter(const sensor_msgs::ImageConstPtr& msg);
    void ros_depth_TOcv_Converter(const sensor_msgs::ImageConstPtr& msg);
    void rgb_info_TOcv_Converter(const sensor_msgs::CameraInfoConstPtr& msg);
    void depth_info_TOcv_Converter(const sensor_msgs::CameraInfoConstPtr& msg);
};


//画像形式
class Image_data{
public:
    Image_data();
    virtual ~Image_data();
    int addImage_data(cv::Mat p);
    int setImage_data(cv::Mat p);
    cv::Mat getImage_data();

private:
    cv::Mat image;
    bool rgbChecker;
};

//入出力クラス
class PostMan{
private:
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Publisher image_pub_;
  int Camera_Size;

public:
  PostMan();
  ~PostMan();
  postData *camera_data;
  int setAddress(void);
  int getCameraSize();
};

//画像処理関数群
namespace ImageProcessing{
    int Depth_to_Image(cv::Mat input,cv::Mat *output,float dd);
    int Calibrate_Depth_Image(cv::Mat depth_image,cv::Mat *output);
    cv::Mat combineImage(cv::Mat input1,cv::Mat input2);
    cv::Mat makeChessboardCorners(int row,int col,float length);
};

#endif