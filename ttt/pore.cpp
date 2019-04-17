//
//  pore.cpp
//  ttt
//
//  Created by 朱瑞 on 17/7/22.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "pore.hpp"

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <math.h>
#include "frangi.hpp"

using namespace std;

string PoreDetection::imgPreprocess(string inpath,string outpath ){
    
    cv::Mat matSrc=cv::imread(inpath,CV_LOAD_IMAGE_UNCHANGED);
    
    cv::cvtColor(matSrc, matSrc, CV_BGR2Lab);
    vector<cv::Mat> matLab;
    cv::split(matSrc, matLab);
    matSrc=matLab[0];
    cv::Mat matDst(matSrc.size(),matSrc.type());
    
    
    
    
    int threshold = 0;
    float amount = 1;
    cv::Mat imgblurred;
    cv::GaussianBlur(matSrc, imgblurred, cv::Size(71,71), 0, 0);
    cv::Mat lowcontrastmask = abs(matSrc - imgblurred)<threshold;
    matDst = matSrc*(1 + amount) + imgblurred*(-amount);
    matSrc.copyTo(matDst, lowcontrastmask);
    
//    float alpha = 1.5;
//    float beta = -20;
//    for (int y = 0; y < matDst.rows; y++)
//    {
//        for (int x = 0; x < matDst.cols; x++)
//        {
//            matDst.at<uchar>(y, x)= cv::saturate_cast<uchar>(alpha*(matDst.at<uchar>(y, x)) + beta);
//        }
//    }
//    
//    cv::equalizeHist(matDst, matDst);
    
    cv::Mat diamond = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
//    cv::morphologyEx(matDst, matDst, cv::MORPH_OPEN, diamond);
    
    
    imwrite(outpath, matDst);
    return outpath;
    

}




string PoreDetection::imgToPore(string inpath, string outpath){
    
    frangi2d_opts_t opt;
    opt.sigma_start=5;
    opt.sigma_end=7;
    opt.sigma_step=1;
    opt.BetaOne=0.5;
    opt.BetaTwo=15;
    opt.BlackWhite=true;
    
    
    
    cv::Mat src=cv::imread(inpath,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat grad;
    string write_path = outpath;
    int scale = 1;
    int delta = 0;
    //    int ddepth = CV_16S;
    int ddepth = src.depth();
    
    
    if( !src.data )
    {
        cout<<"load gray image error!"<<endl;
        return NULL;
    }
    
    
    GaussianBlur( src, src, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    cv::Mat grad_x(src.size(),CV_32FC1), grad_y(src.size(),CV_32FC1);
    cv::Mat abs_grad_x(src.size(),CV_32FC1), abs_grad_y(src.size(),CV_32FC1);
    
    /// 求 X方向梯度
    //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    cv::Sobel( src, grad_x, CV_32FC1, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grad_x, abs_grad_x );
    
    /// 求Y方向梯度
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    cv::Sobel( src, grad_y, CV_32FC1, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grad_y, abs_grad_y );
    
    /// 合并梯度(近似)
    grad.convertTo(grad, CV_32FC1);
    cv::Mat angel1(src.size(),CV_32FC1);
    abs_grad_y.convertTo(abs_grad_y, CV_32FC1);
    abs_grad_x.convertTo(abs_grad_x, CV_32FC1);
    cv::cartToPolar(abs_grad_x, abs_grad_y, grad, angel1);
//    cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
    cv::imwrite("/Users/zrain/Desktop/pic/pore_mid/grad_.jpg", grad);
    cv::imwrite("/Users/zrain/Desktop/pic/pore_mid/angel_.jpg", angel1);

    
    cv::Mat imggrident=src;
    
    cv::Mat scales(imggrident.size(),CV_32FC1);
    cv::Mat directions(imggrident.size(),CV_32FC1);
    cv::Mat nsrc(imggrident.size(),CV_32FC1);
    
    imggrident.convertTo(nsrc, CV_32FC1);
    cv::Mat outfrangi(imggrident.size(),CV_32FC1);
    
    pore_2d(nsrc,outfrangi,scales,directions,opt);
    cv::Mat dest=cv::Mat::zeros(outfrangi.size(), CV_32FC1);
    cv::imwrite("/Users/zrain/Desktop/pic/pore_mid/outfrangi_.jpg", outfrangi);


    cv::Mat mask=abs(directions-angel1)<1.14154;
//    for(int i=0;i<50;i++)
//        cout<<(int)mask.at<uchar>(i,i)<<" ";
//    cv::imwrite("/Users/zrain/Desktop/pic/pore_mid/mask_.jpg", mask);

//    outfrangi.copyTo(dest,mask);
    
    cv::threshold(outfrangi, outfrangi, 0.0, 255,CV_THRESH_BINARY );
    
    cv::imwrite(outpath, outfrangi);
    return outpath;

  

    
}

void PoreDetection::showPoreInit(string init,string marked,string outpath){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(marked,CV_LOAD_IMAGE_UNCHANGED);
//    cv::morphologyEx(mark, mark, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
//    cv::morphologyEx(mark, mark, cv::MORPH_ERODE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
    
    

    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
//            cout<<(int)mark.at<uchar>(r,c)<<",";
            if((int)mark.at<uchar>(r,c)>200){
                src.at<cv::Vec3b>(r,c)[0]=255;
                src.at<cv::Vec3b>(r,c)[1]=0;
                src.at<cv::Vec3b>(r,c)[2]=0;
                
                
            }
            
        }
    cv::imwrite(outpath, src);
    
}


void PoreDetection::showPoreLine(int minlimit,int maxlimit,string init,string marked,string outpath){
    cout<<"begin to draw!"<<endl;
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(marked,CV_LOAD_IMAGE_UNCHANGED);

    
    //filter small
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
        if(circle_area>0.4 && radius>3 && radius <15 && cv::contourArea(contours[i])>minlimit && cv::contourArea(contours[i])<maxlimit)
        {
//            circle(mask,center,radius,cv::Scalar(255),2);
            contours_big.push_back(contours[i]);
        }
    }

    cv::drawContours(mask, contours_big, -1, cv::Scalar(255));

    cv::threshold(mask , mask, 20, 255,CV_THRESH_BINARY );

//    cv::Mat outimage;
//    mark.copyTo(outimage, mask);


//    cv::morphologyEx(mark,mark,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
//    cv::threshold(outimage, outimage, 20, 255,CV_THRESH_BINARY );
    
    //begin to draw
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            if((int)mask.at<uchar>(r,c)>220){
                src.at<cv::Vec3b>(r,c)[0]=250;
                src.at<cv::Vec3b>(r,c)[1]=25;
                src.at<cv::Vec3b>(r,c)[2]=100;
                
                
            }
            
        }
    cv::imwrite(outpath, src);
    
}

void PoreDetection::drawPoreBlack(int minlimit,int maxlimit,string init,string gray,string out){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(gray,CV_LOAD_IMAGE_UNCHANGED);
    cv::morphologyEx(mark,mark,cv::MORPH_ERODE,getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)));
    cv::morphologyEx(mark,mark,cv::MORPH_DILATE,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    
    vector<vector<cv::Point>> contours;
    vector<vector<cv::Point>> contours_out;
    vector<cv::Vec4i> hierarchy;
    cv::findContours(mark, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultImage = cv::Mat ::zeros(mark.size(),CV_8U);
    for(int i=0;i<contours.size();i++)
    {
        if(cv::contourArea(contours[i])>minlimit && cv::contourArea(contours[i])<maxlimit)
            contours_out.push_back(contours[i]);
        
    }
    cv::drawContours(resultImage, contours_out, -1, cv::Scalar(255));
    cv::imwrite(out, resultImage);
    
    
}

void PoreDetection::drawPoreRaw(int minlimit,int maxlimit,string init,string gray,string out){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(gray,CV_LOAD_IMAGE_UNCHANGED);
    cv::morphologyEx(mark,mark,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    vector<vector<cv::Point>> contours;
    vector<vector<cv::Point>> contours_out;
    vector<cv::Vec4i> hierarchy;
    findContours(mark, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultImage = cv::Mat ::zeros(mark.size(),CV_8U);
    for(int i=0;i<contours.size();i++)
    {
        if(cv::contourArea(contours[i])>minlimit && cv::contourArea(contours[i])<maxlimit)
            contours_out.push_back(contours[i]);
        
    }
    cv::drawContours(resultImage, contours_out, -1, cv::Scalar(255));
    
    for(int r=0;r<mark.rows;r++)
        for(int c=0;c<mark.cols;c++)
        {
            if(resultImage.at<uchar>(r,c)==255)
            {
                src.at<cv::Vec3b>(r,c)[0]=255;
                src.at<cv::Vec3b>(r,c)[1]=50;
                src.at<cv::Vec3b>(r,c)[2]=20;
            }
        }
    cv::imwrite(out, src);
    
    
}

