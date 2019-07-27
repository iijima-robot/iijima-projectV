#include "ClassDefinition.h"

//image_dataクラスはMat行列を取り扱う
Image_data::Image_data(){}
Image_data::~Image_data(){}

int Image_data::setImage_data(cv::Mat p){
    if(p.empty()){
        return -1;
    }
    image=p.clone();
    return 0;
}

cv::Mat Image_data::getImage_data(){
   return image;
}

