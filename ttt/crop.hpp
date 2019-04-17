//
//  crop.hpp
//  ttt
//
//  Created by 朱瑞 on 17/7/10.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#ifndef crop_hpp
#define crop_hpp

#include<string>
#include<opencv2/opencv.hpp>


int cropImage(std::string filename, int point, cv::Rect rec1,cv::Rect rec2,cv::Rect rec3) ;
int markImagePoints(std::string filename,std::string outpath) ;
void drawR33Eye(cv::Mat &roi, std::vector<cv::Point> points);
void drawL33Eye(cv::Mat roi, std::vector<cv::Point> points);
int cropImageROI(std::string filename,std::string outpath) ;
std::vector<cv::Point> getImagePoint(std::string filename) ;
void drawLips(cv::Mat &roi, std::vector<cv::Point> points);
void curve4(std::vector<cv::Point> &p,
            double x1, double y1,   //Anchor1
            double x2, double y2,   //Control1
            double x3, double y3,   //Control2
            double x4, double y4);

void cropAcne(std::string fileRgb,std::string fileGray,std::string outpath);

#endif 
/* crop_hpp */
