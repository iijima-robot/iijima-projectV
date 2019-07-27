#include "ClassDefinition.h"

//距離->カラー画像
int ImageProcessing::Depth_to_Image(cv::Mat input,cv::Mat *output,float dd){
  cv::Mat ans(input.size(), CV_8UC3, cv::Scalar(0,255,0));
  for(int y=0;y<input.rows;y++){
    cv::Vec3b *src = ans.ptr<cv::Vec3b>(y);
    for(int x=0;x<input.cols;x++){
      float d = input.at<float>(y,x);
      if(d==0){
        src[x][0] = 0;
        src[x][1] = 0;
        src[x][2] = 0;
      }else if(d<dd/10){
        src[x][0] = 25;
        src[x][1] = 25;
        src[x][2] = 255;
      }else if(d<dd*2/10){
        src[x][0] = 25;
        src[x][1] = 163;
        src[x][2] = 255;
      }else if(d<dd*3/10){
        src[x][0] = 25;
        src[x][1] = 255;
        src[x][2] = 209;
      }else if(d<dd*4/10){
        src[x][0] = 25;
        src[x][1] = 255;
        src[x][2] = 71;
      }else if(d<dd*5/10){
        src[x][0] = 125;
        src[x][1] = 254;
        src[x][2] = 25;
      }else if(d<dd*6/10){
        src[x][0] = 255;
        src[x][1] = 255;
        src[x][2] = 25;
      }else if(d<dd*7/10){
        src[x][0] = 255;
        src[x][1] = 117;
        src[x][2] = 25;
      }else if(d<dd*8/10){
        src[x][0] = 255;
        src[x][1] = 25;
        src[x][2] = 71;
      }else if(d<dd*9/10){
        src[x][0] = 255;
        src[x][1] = 25;
        src[x][2] = 209;
      }else {
        src[x][0] = 163;
        src[x][1] = 25;
        src[x][2] = 255;
      }
      
    }
  }
  
  *output = ans.clone();
  return 0;
}

int ImageProcessing::Calibrate_Depth_Image(cv::Mat depth_image,cv::Mat *output){
  //各変数
  const float WHRatio       = 1;
  const float inScaleFactor = 0.007843f;
  //
  //カラー画像と画像座標系を一致させる
  //set par
  cv::Size cropSize;
/*
  cropSize = cv::Size(static_cast<int>(depth_image.cols * WHRatio),
  depth_image.cols);
  
  //Set size
   cropSize = cv::Size(640,480);
  cv::Rect crop(cv::Point((depth_image.rows - cropSize.width) / 2,
                    (depth_image.cols - cropSize.height) / 2),
              cropSize);
             
  //depth_image.convertTo(depth_image, CV_64F);
  if(!depth_image.empty()){
    cv::resize(depth_image,*output,cv::Size(),0,0,cv::INTER_NEAREST);
  }
*/
  return 0;
}

//画像結合
cv::Mat ImageProcessing::combineImage(cv::Mat input1,cv::Mat input2){
  //出力Mat作成
  int cols = input1.cols+input2.cols;
  int rows = input1.rows;
  cv::Mat ans = cv::Mat(rows,cols,CV_8UC3);

  //結合
  for(int y=0;y<rows;y++){
    cv::Vec3b *src1 = input1.ptr<cv::Vec3b>(y);
    cv::Vec3b *src2 = input2.ptr<cv::Vec3b>(y);
    cv::Vec3b *out = ans.ptr<cv::Vec3b>(y);
    for(int x=0;x<input1.cols;x++){
      out[x]=src1[x];
      out[x+input1.cols]=src2[x];
    }
  }

  return ans;
}

//チェスボート作成
cv::Mat ImageProcessing::makeChessboardCorners(int row,int col,float length){
  cv::Mat ans = cv::Mat(row,col,CV_32FC3);
  for(int y=0;y<row;y++){
    cv::Vec3f *out = ans.ptr<cv::Vec3f>(y);
    for(int x=0;x<col;x++){
      out[x][0]=x*length;
      out[x][1]=y*length;
      out[x][2]=0;
    }
  }
  return ans;
}
