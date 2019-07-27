#include "ClassDefinition.h"

//postManクラスはパッケージの送受信を司る
//各関数の概要についてはヘッダファイルを参照

//コンストラクタ
PostMan::PostMan(): it_(nh_){
    //rs2::pipeline pipe;
  //auto profile = pipe.start(cfg);
  //auto depth_stream = profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
 // auto depth_intr = depth_stream.get_intrinsics();
}
PostMan::~PostMan(){
    delete[] camera_data;
}

//送受信設定
int PostMan::setAddress(void){
    ros::NodeHandle launch_node("~");
    //カメラ名設定
    Camera_Size = 2;
    launch_node.getParam("camera_Size",Camera_Size);
    //配列作成
    camera_data = new postData[Camera_Size];
    for(int i=0;i<Camera_Size;i++){
        //トピック名生成
        std::string camera_name = "camera"+std::to_string((i+1));
        camera_data[i].camera.camera_name=camera_name;
        
        //camera_name=camera_name+"_name";
        launch_node.getParam(camera_name,camera_data[i].camera.camera_name);
        //受信部
        camera_data[i].getData(nh_,it_);
    }

    //送信部
    //image_pub_ = it_.advertise("output_video", 1);
   
   return 0;
}
//受信部
void postData::getData(ros::NodeHandle nh,image_transport::ImageTransport it){
    ROS_INFO_STREAM(camera.camera_name);
    image_sub_ = it.subscribe(camera.camera_name+"/color/image_raw", 1,
      &postData::ros_RGB_TOcv_Converter,this);
    depth_image = it.subscribe(camera.camera_name+"/depth/image_raw",1,&postData::ros_depth_TOcv_Converter,this);
    rgb_info = nh.subscribe(camera.camera_name+"/color/camera_info",1,&postData::rgb_info_TOcv_Converter,this);
    depth_info = nh.subscribe(camera.camera_name+"/depth/camera_info",1,&postData::depth_info_TOcv_Converter,this);
}

//画像形式変換
void postData::ros_RGB_TOcv_Converter(const sensor_msgs::ImageConstPtr& msg){
    cv_bridge::CvImagePtr cv_ptr;
    try{
        //ROS形式からOpenCV形式へ変換
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

        //Gazouとして登録
        camera.RGB_Gazou = cv_ptr->image;
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }    
}

void postData::ros_depth_TOcv_Converter(const sensor_msgs::ImageConstPtr& msg){
    cv_bridge::CvImagePtr cv_ptr;
    try{
        //ROS形式からOpenCV形式へ変換
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1);        
        //Depth_Gazouとして登録
        camera.Depth_Gazou = cv_ptr->image;
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
}

void postData::rgb_info_TOcv_Converter(const sensor_msgs::CameraInfoConstPtr& msg){
    camera.RGB_INFO.width =(int)msg->width;
    camera.RGB_INFO.height =(int)msg->height;
    ROS_INFO_THROTTLE(2,"RGB-width:%d",camera.RGB_INFO.width);
}
void postData::depth_info_TOcv_Converter(const sensor_msgs::CameraInfoConstPtr& msg){
    camera.Depth_INFO.width =(int)msg->width;
    camera.Depth_INFO.height =(int)msg->height;
    ROS_INFO_THROTTLE(2,"D-width:%d",camera.Depth_INFO.width);
}

int PostMan::getCameraSize(){
    return Camera_Size;
}