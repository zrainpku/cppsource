//
//  smooth.cpp
//  ttt
//
//  Created by 朱瑞 on 17/7/29.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "smooth.hpp"
#include "frangi.hpp"
#include "frame.hpp"
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <math.h>
#include <strstream>


using namespace std;

void SmoothDetection::imgProcess(std::string read_path,std::string write_path)
{
    cv::Mat matSrc=cv::imread(read_path,CV_LOAD_IMAGE_UNCHANGED);
    cv::cvtColor(matSrc, matSrc, CV_BGR2Lab);
    vector<cv::Mat> labimg;
    cv::split(matSrc, labimg);
    cv::Mat matDst(matSrc.size(),CV_8U);
    matSrc=labimg[0];
    
    int threshold = 0;
    float amount = 5;
    cv::Mat imgblurred;
    cv::GaussianBlur(matSrc, imgblurred, cv::Size(51, 51), 0, 0);
    cv::Mat lowcontrastmask = abs(matSrc - imgblurred)<threshold;
    matDst = matSrc*(1 + amount) + imgblurred*(-amount);
    matSrc.copyTo(matDst, lowcontrastmask);
    
    float alpha = 1.5;
    float beta = -20;
    for (int y = 0; y < matDst.rows; y++)
    {
        for (int x = 0; x < matDst.cols; x++)
        {
            matDst.at<uchar>(y, x)= cv::saturate_cast<uchar>(alpha*(matDst.at<uchar>(y, x)) + beta);
        }
    }
    
//    cv::equalizeHist(matDst, matDst);
    
    cv::Mat diamond = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(matDst, matDst, cv::MORPH_OPEN, diamond);
    cv::imwrite(write_path+"smooth_temp_gray.bmp", matDst);
    
    //end gray
    
    cv::Mat grad;
    int scale = 1;
    int delta = 0;
    int ddepth = matDst.depth();
    if( !matDst.data )
    {
        cout<<"load gray image error!"<<endl;
        return ;
    }
    
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    cv::Sobel( matDst, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    
    cv::Sobel( matDst, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    
    grad_x.convertTo(abs_grad_x, CV_32FC1);
    grad_y.convertTo(abs_grad_y, CV_32FC1);
    cv::Mat temp;
    cv::cartToPolar(abs_grad_x, abs_grad_y, grad, temp);
    
//        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
    addWeighted( abs_grad_x, 0.8, abs_grad_y, 0.2, 0, grad_x );
    addWeighted( abs_grad_x, 0.2, abs_grad_y, 0.8, 0, grad_y );
    
    grad.setTo(0);
    cv::Mat check=cv::abs(abs_grad_x)<cv::abs(abs_grad_y);
    abs_grad_x.copyTo(grad);
    abs_grad_y.copyTo(grad,check);
    
    cv::imwrite(write_path+"smooth_temp_abs.bmp", grad);
    cv::imwrite(write_path+"smooth_temp_abs_x.bmp", abs_grad_x);
    cv::imwrite(write_path+"smooth_temp_abs_y.bmp", abs_grad_y);
    
    //end grident
    
    frangi2d_opts_t opt;
    opt.sigma_start=3;
    opt.sigma_end=9;
    opt.sigma_step=2;
    opt.BetaOne=0.5;
    opt.BetaTwo=15;
    opt.BlackWhite=true;
    
    
    cv::Mat scales(grad.size(),CV_32FC1);
    cv::Mat directions(grad.size(),CV_32FC1);
    cv::Mat src(grad.size(),CV_32FC1);
    
    grad.convertTo(src, CV_32FC1);
    cv::Mat outfrangi(grad.size(),CV_32FC1);
    frangi2d(src,outfrangi,scales,directions,opt);
    
    cv::Mat src_x(grad.size(),CV_32FC1);
    grad_x.convertTo(src_x, CV_32FC1);
    cv::Mat outfrangi_x(grad.size(),CV_32FC1);
    frangi2d(src_x,outfrangi_x,scales,directions,opt);
    
    cv::Mat src_y(grad.size(),CV_32FC1);
    grad_y.convertTo(src_y, CV_32FC1);
    cv::Mat outfrangi_y(grad.size(),CV_32FC1);
    frangi2d(src_y,outfrangi_y,scales,directions,opt);
    //end frangi
    cv::Mat out(grad.size(),CV_8U),out_x(grad.size(),CV_8U),out_y(grad.size(),CV_8U);
    outfrangi.convertTo(outfrangi, CV_8U);
    outfrangi_x.convertTo(outfrangi_x, CV_8U);
    outfrangi_y.convertTo(outfrangi_y, CV_8U);
    
    cv::threshold(outfrangi, out, 0, 255,CV_THRESH_BINARY );
    cv::threshold(outfrangi_x, out_x, 0, 255,CV_THRESH_BINARY );
    cv::threshold(outfrangi_y, out_y, 0, 255,CV_THRESH_BINARY );
    
    
    cv::imwrite(write_path+"smooth_ridge.bmp", out);
    cv::imwrite(write_path+"smooth_ridge_x.bmp", out_x);
    cv::imwrite(write_path+"smooth_ridge_y.bmp", out_y);
    




}


void SmoothDetection::showSmoothCommonInit(string init,string markedpath,string outpath){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(markedpath+"smooth_ridge.bmp",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat mark_x=cv::imread(markedpath+"smooth_ridge_x.bmp",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat mark_y=cv::imread(markedpath+"smooth_ridge_y.bmp",CV_LOAD_IMAGE_GRAYSCALE);
    
    
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            if(r<0)
                cout<<(int)mark.at<uchar>(r,c)<<",";
            if((int)mark.at<uchar>(r,c)>200 ||(int)mark_x.at<uchar>(r,c)>200 ||(int)mark_y.at<uchar>(r,c)>200){
                src.at<cv::Vec3b>(r,c)[0]=200;
                src.at<cv::Vec3b>(r,c)[1]=50;
                src.at<cv::Vec3b>(r,c)[2]=20;
                
                
            }
            
        }
    cv::imwrite(outpath, src);
    
}


void SmoothDetection::showSmoothCommonLine(int minlimit,int maxlimit, string init,string markedpath,string outpath){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(markedpath+"smooth_ridge.bmp",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat mark_x=cv::imread(markedpath+"smooth_ridge_x.bmp",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat mark_y=cv::imread(markedpath+"smooth_ridge_y.bmp",CV_LOAD_IMAGE_GRAYSCALE);
    
    
    
    cv::Mat markAll(mark.size(), mark.type());
    markAll.setTo(0);
    
    
    
    //****************************
    for(int ite=0;ite<1;ite++){
        if(ite==0)mark=mark;
        else if (ite==1)mark=mark_y;
        else mark=mark;
        

        cv::medianBlur(mark, mark, 3);
        cv::morphologyEx(mark,mark,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
//        cv::morphologyEx(mark,mark,cv::MORPH_DILATE,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        
        //    cv::morphologyEx(mark,mark,MORPH_OPEN,elem);
        cv::threshold(mark, mark, 100, 255,CV_THRESH_BINARY );
        
        //filter
        vector<vector<cv::Point>> contours;
        vector<cv::Vec4i> hierarchy;
        cv::Mat mark2=mark.clone();
        
        findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
        cv::Mat resultMask =cv:: Mat ::zeros(mark.size(),CV_8U);
        cv::Mat resultImage = cv::Mat ::zeros(mark.size(),CV_8U);
        vector<vector<cv::Point>> contours_small;
        contours_small.clear();
        for(int i=0;i<contours.size();i++)
        {
            if(cv::contourArea(contours[i])>minlimit && cv::contourArea(contours[i])<maxlimit )
                contours_small.push_back(contours[i]);
            
        }
        
        
        drawContours(resultMask, contours_small, -1, cv::Scalar(255),CV_FILLED);
        cv::threshold(resultMask, resultMask, 0, 255, CV_THRESH_BINARY);
        
        mark.copyTo(resultImage, resultMask);
        //        changeToframe(resultImage);
        
        
        markAll=markAll | resultImage;
        
    }//for
    //*********************************
    
    cv::Mat resultImage =cv:: Mat ::zeros(mark.size(),CV_8U);
//    cv::morphologyEx(mark,mark,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    
//    mark.copyTo(resultImage, markAll);
    //    cv::morphologyEx(resultImage,resultImage,MORPH_DILATE,getStructuringElement(MORPH_RECT, Size(3,3)));
    //    cv::morphologyEx(markAll,markAll,MORPH_OPEN,getStructuringElement(MORPH_RECT, Size(3, 3)));
    
//    cv::morphologyEx(markAll,markAll,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    //    cv::threshold(markAll, markAll, 0, 255, CV_THRESH_BINARY);
    
 
    long long counts=0;
    //begin to draw
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            //            cout<<(int)mark.at<uchar>(r,c)<<"--";
            if((int)markAll.at<uchar>(r,c)>225){
                counts++;
            }
            
        }
    changeToframe(markAll);

    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            //            cout<<(int)mark.at<uchar>(r,c)<<"--";
            if((int)markAll.at<uchar>(r,c)>225){
                src.at<cv::Vec3b>(r,c)[0]=0;
                src.at<cv::Vec3b>(r,c)[1]=255;
                src.at<cv::Vec3b>(r,c)[2]=0;
                
                
            }
            
        }
    float ans=0;
    ans=(float)counts/((float)src.rows*src.cols);
    ans=1-ans;
    ans*=100;
    string title="smooth score is: (best is 100, worst is 0)";
    strstream ss; string s; ss << ans; ss >> s;
    cv::putText(src, title, cv::Point(100,100), 1, 5.0f, cv::Scalar(255, 0, 0),4,8);
    cv::putText(src, s, cv::Point(100,200), 1, 5.0f, cv::Scalar(255, 0, 0),4,8);
    
    cv::imwrite(outpath, src);
    
}
