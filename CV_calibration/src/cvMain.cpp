#include "cvMain.h"

  cvMain::cvMain(){
    // Subscrive to input video feed and publish output video feed
    pm_.setAddress();
    rgb=0;    
    OPENCV_WINDOW = pm_.camera_data[0].camera.camera_name+" RGB window";
    Camera_WINDOW = pm_.camera_data[0].camera.camera_name+" window";
    CV_WINDOW = pm_.camera_data[0].camera.camera_name+" chess window";
    cv::namedWindow(OPENCV_WINDOW);
    cv::namedWindow(Camera_WINDOW);
    
  }

  cvMain::~cvMain()
  {
    cv::destroyWindow(OPENCV_WINDOW);
    cv::destroyWindow(Camera_WINDOW);
  }


  int cvMain::cvMain_Function(){
    //NULLチェック
    for(int i=0;i<pm_.getCameraSize();i++){
      if(pm_.camera_data[i].camera.RGB_Gazou.empty() || pm_.camera_data[i].camera.Depth_Gazou.empty()){
      return -1;
      }
    }

    //変数一覧
    cv::Mat d_image;//カラー化された距離画像
    cv::Mat cv_image[2];//画像処理用配列

    // Convert Depth Image
    ImageProcessing::Depth_to_Image(pm_.camera_data[0].camera.Depth_Gazou,&d_image,5000);
    

    // 画像処理
    if(rgb<50){
      cv::circle(pm_.camera_data[0].camera.RGB_Gazou, cv::Point(pm_.camera_data[0].camera.RGB_Gazou.cols/2,pm_.camera_data[0].camera.RGB_Gazou.rows/2), 10, CV_RGB(255,255,255),3);
    }else if(rgb<60){
      cv::circle(pm_.camera_data[0].camera.RGB_Gazou, cv::Point(pm_.camera_data[0].camera.RGB_Gazou.cols/2,pm_.camera_data[0].camera.RGB_Gazou.rows/2), 10, CV_RGB(0,255,0),3);
    }else{
      //チェスボート検出
      std::vector<std::vector<cv::Point2f>> corners;
      for(int i=0;i<pm_.getCameraSize();i++){
        cv_image[i]=pm_.camera_data[i].camera.RGB_Gazou.clone();
        corners.push_back(std::vector<cv::Point2f> ());
        cv::findChessboardCorners(cv_image[i],CvSize(10,7),corners[i],CV_CALIB_CB_ADAPTIVE_THRESH);
        cv::drawChessboardCorners(cv_image[i],CvSize(10,7),cv::Mat(corners[i]),true);
      }

      //画像結合
      output_image = ImageProcessing::combineImage(cv_image[0],cv_image[1]);

      //チェスボート検知時
      if(corners[0].size()>0&&corners[1].size()>0){

      //チェスボート作成
      //chess_object.push_back(ImageProcessing::makeChessboardCorners(10,7,24));

      //内部パラメータ・歪み検出
      /*
      cv::Mat cameraMatrix;
      cv::Mat distCoeffs;
      cv::Mat rvecs;
      cv::Mat tvecs;
      */
      //cv::CalibrateCamera2(chess_object,corners[0],corners[0].size(),cv::Size(640,480),cameraMatrix,tvecs,0);

     
      
      //出力
      
        float d0 = pm_.camera_data[0].camera.Depth_Gazou.at<float>((int)corners[0][0].x,(int)corners[0][0].y);
        float d1 = pm_.camera_data[1].camera.Depth_Gazou.at<float>((int)corners[1][0].x,(int)corners[1][0].y);
        ROS_INFO("read>>(%5.1f,%5.1f,%5.1f),(%5.1f,%5.1f,%5.1f)",corners[0][0].x,corners[0][0].y,d0,corners[1][0].x,corners[1][0].y,d1);
      }else{
        ROS_INFO("size>>(%d,%d)",(int)corners[0].size(),(int)corners[1].size());
      }
      //撮影マーカー表示
      cv::circle(pm_.camera_data[0].camera.RGB_Gazou, cv::Point(pm_.camera_data[0].camera.RGB_Gazou.cols/2,pm_.camera_data[0].camera.RGB_Gazou.rows/2), 10, CV_RGB(255,0,0),3);
      //時間計測初期化
      rgb=0;
    }
    cv::circle(d_image, cv::Point(d_image.cols/2,d_image.rows/2), 10, CV_RGB(255,255,255),3);
    
    
    // Update GUI Window    
    cv::imshow(OPENCV_WINDOW, pm_.camera_data[0].camera.RGB_Gazou);
    cv::imshow(Camera_WINDOW, d_image);
    if(!output_image.empty()){
      cv::imshow(CV_WINDOW, output_image);
    }else{
      cv::imshow(CV_WINDOW, d_image);
    }
    cv::waitKey(100);
    
    //時間計測
    rgb++;    
    // Output modified video stream
    //image_pub_.publish(cv_ptr->toImageMsg());
    return 0;
  }
/*
  void cvMain::imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // Draw an example circle on the video stream
    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      cv::circle(cv_ptr->image, cv::Point(rgb, 50), 40, CV_RGB(255,0,0));

    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);

    rgb++;
    if(rgb==600){
        rgb=0;

    }
    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
*/