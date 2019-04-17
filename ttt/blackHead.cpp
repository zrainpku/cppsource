//
//  blackHead.cpp
//  ttt
//
//  Created by 朱瑞 on 17/8/23.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "blackHead.hpp"
#include <string>
#include<algorithm>
#include<opencv2/opencv.hpp>
using namespace std;

int max_int(int a,int b){

    return a>b?a:b;
}

int min_int(int a,int b){
    
    return a>b?b:a;
}

void findBlackHead(std::string srcpath,std::string markpath,std::string outpath){

    cv::Mat src=cv::imread(srcpath);
    cv::Mat labImg;
    cv::cvtColor(src, labImg, CV_BGR2Lab);
    vector<cv::Mat> channels;
    cv::split(labImg, channels);
    cv::Mat gray=channels[0];
    cv::Mat mean_rect=gray(cv::Rect(10,10,400,400));
    float all_mean=cv::mean(mean_rect).val[0];
    cv::Mat mark=cv::imread(markpath,CV_LOAD_IMAGE_UNCHANGED);
    
    cv::Mat mark2=mark.clone();
    vector<vector<cv::Point>> contours;
    vector<vector<cv::Point>> contours_big;
    vector<cv::Vec4i> hierarchy;
    cv::morphologyEx(mark2, mark2, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
    cv::threshold(mark2 , mark2, 100, 255,CV_THRESH_BINARY );
    
    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    
    cv::Mat mask(mark.size(),CV_8UC1);
    mask.setTo(0);
    for(int i=0;i<contours.size();i++)
    {
        cv::Rect rec=cv::boundingRect(contours[i]);
        cv::Point2f center; float radius;
        minEnclosingCircle(contours[i],center,radius);
        int temp_height=rec.height;
        int temp_width=rec.width;
        temp_width=((temp_width>temp_height)?temp_width:temp_height);
        float circle_area=3.1415926*radius*radius;
        float true_area=cv::contourArea(contours[i]);
        circle_area=true_area/circle_area;
        if(circle_area>0.4 && radius>3 && radius <15 && cv::contourArea(contours[i])>30 && cv::contourArea(contours[i])<400)
        {
            int edge1=radius/2;
            int topx=max_int(center.x-edge1, 0.0);
            int topy=max_int(center.y-edge1, 0.0);
            int topwidth=min_int(edge1,mask.cols-topx);
            int topheight=min_int(edge1, mask.rows-topy);

            cv::Rect rec1(topx,topy,topwidth,topheight);
            int edge2=radius*2;
            topx=max_int(center.x-edge2, 0.0);
            topy=max_int(center.y-edge2, 0.0);
            topwidth=min_int(edge2,mask.cols-topx);
            topheight=min_int(edge2, mask.rows-topy);
            
            cv::Rect rec2(topx,topy,topwidth,topheight);
            cv::Mat crop_mean1=gray(rec1);
            cv::Mat crop_mean2=gray(rec2);
            float pore_mean1=cv::mean(crop_mean1).val[0];
            float pore_mean2=cv::mean(crop_mean2).val[0];
            if(pore_mean1<pore_mean2-13)
            contours_big.push_back(contours[i]);
        }
    }
    
    cv::drawContours(mask, contours_big, -1, cv::Scalar(255));
    
    cv::threshold(mask , mask, 20, 255,CV_THRESH_BINARY );
    
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            if((int)mask.at<uchar>(r,c)>220){
                src.at<cv::Vec3b>(r,c)[0]=250;
                src.at<cv::Vec3b>(r,c)[1]=25;
                src.at<cv::Vec3b>(r,c)[2]=10;
                
                
            }
            
        }
    cv::imwrite(outpath, src);
    
    
    
    return;
}
