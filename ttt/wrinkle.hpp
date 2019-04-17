//
//  wrinkle.hpp
//  ttt
//
//  Created by 朱瑞 on 17/8/24.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#ifndef wrinkle_hpp
#define wrinkle_hpp

#include <string>
#include<iostream>
#include <opencv2/opencv.hpp>

class KWrinkleDetect{
    
private:
    cv::Mat matSrc;
    cv::Mat matTemp;
    cv::Mat matAns;
    int minlimit;
    int maxlimit;
    cv::Mat imgToRidge(cv::Mat mat);
    cv::Mat showWrinkleCommonLine();

    
    
public:
    

    KWrinkleDetect(cv::Mat mat);
    KWrinkleDetect(cv::Mat mat,const int min_limit);
    KWrinkleDetect(cv::Mat mat,const int min_limit,const int max_limit);

    ~KWrinkleDetect();
    cv::Mat getOutputImage();

//    void imgToRidge(std::string inimage,std::string write_path);
//    void showWrinkleCommonLine(int minlimit,int maxlimit,std::string init,std::string markedpath,std::string outpath);
    
    
};//class


#endif /* wrinkle_hpp */
