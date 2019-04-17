//
//  wrinkle.cpp
//  ttt
//
//  Created by 朱瑞 on 17/8/24.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "wrinkle.hpp"
#include "frangi.hpp"
#include "frame.hpp"
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <math.h>

using namespace std;



KWrinkleDetect::KWrinkleDetect(cv::Mat mat){
    minlimit=200;
    maxlimit=99999;
    matSrc=mat.clone();
    matTemp=imgToRidge(mat);
    matAns=showWrinkleCommonLine();
    
}
KWrinkleDetect::KWrinkleDetect(cv::Mat mat,int min_limit){
    minlimit=min_limit;
    maxlimit=99999;
    matSrc=mat.clone();
    matTemp=imgToRidge(mat);
    matAns=showWrinkleCommonLine();
    
}

KWrinkleDetect::KWrinkleDetect(cv::Mat mat,int min_limit,const int max_limit){
    minlimit=min_limit;
    maxlimit=max_limit;
    matSrc=mat.clone();
    matTemp=imgToRidge(mat);
    matAns=showWrinkleCommonLine();
    
}


KWrinkleDetect::~KWrinkleDetect(){
}


cv::Mat KWrinkleDetect::getOutputImage(){
    return matAns;
}

cv::Mat KWrinkleDetect::imgToRidge(cv::Mat mat){
    
    cv::Mat matSrc1=mat.clone();
//    cv::cvtColor(matSrc, matSrc, CV_BGR2GRAY);
    cv::Mat matDst(matSrc1.size(),CV_8U);
        vector<cv::Mat> labimg;
        cv::split(matSrc1, labimg);
        matSrc1=labimg[0];
    
        int threshold = 0;
    float amount = 3;
    cv::Mat imgblurred;
    cv::GaussianBlur(matSrc1, imgblurred, cv::Size(81, 81), 0, 0);
    cv::Mat lowcontrastmask = abs(matSrc1 - imgblurred)<threshold;
    matDst = matSrc1*(1 + amount) + imgblurred*(-amount);
    matSrc1.copyTo(matDst, lowcontrastmask);
    
    float alpha = 1.5;
    float beta = -10;
    for (int y = 0; y < matDst.rows; y++)
    {
        for (int x = 0; x < matDst.cols; x++)
        {
            matDst.at<uchar>(y, x)= cv::saturate_cast<uchar>(alpha*(matDst.at<uchar>(y, x)) + beta);
        }
    }
    
    //        cv::equalizeHist(matDst, matDst);
    
    cv::Mat diamond = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(matDst, matDst, cv::MORPH_OPEN, diamond);
    
    
    //end gray
    
    
    int scale = 1;
    int delta = 0;
    if( !matDst.data )
    {
        cout<<"load gray image error!"<<endl;
        return matDst;
    }
    
    cv::Mat grad,grad_temp;;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    
    cv::Sobel( matDst, grad_x, matDst.depth(), 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    
    cv::Sobel( matDst, grad_y, matDst.depth(), 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    
    
//    grad.setTo(0);
//    cv::Mat check=cv::abs(abs_grad_x)<cv::abs(abs_grad_y);
//    abs_grad_x.copyTo(grad);
//    abs_grad_y.copyTo(grad,check);
    
    
    grad.convertTo(grad, CV_32FC1);
    grad_x.convertTo(abs_grad_x, CV_32FC1);
    grad_y.convertTo(abs_grad_y, CV_32FC1);
    cv::Mat angle;
    cv::cartToPolar(abs_grad_x, abs_grad_y, grad, angle);
    
//    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/wrinkle_temp_abs.bmp", grad);
    
  //end grident
    
    frangi2d_opts_t opt;
    opt.sigma_start=3;
    opt.sigma_end=6;
    opt.sigma_step=1;
    opt.BetaOne=0.5;
    opt.BetaTwo=15;
    opt.BlackWhite=true;
    
    
    cv::Mat scales(grad.size(),CV_32FC1);
    cv::Mat directions(grad.size(),CV_32FC1);
    cv::Mat src(grad.size(),CV_32FC1);
    
    grad.convertTo(src, CV_32FC1);
    cv::Mat outfrangi;
    frangi2d(src,outfrangi,scales,directions,opt);
    outfrangi.convertTo(outfrangi, CV_8UC1);

    //end frangi
    cv::Mat out,out_x(grad.size(),CV_8U),out_y(grad.size(),CV_8U);
        outfrangi.convertTo(outfrangi, CV_8UC1);

    
    cv::threshold(outfrangi, out, 0, 255,CV_THRESH_BINARY);
//        cv::medianBlur(out, out, 3);
    
//    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/wrinkle_outfrangi.bmp", out);

    return out;

    
    
    
}





bool cmp(vector<cv::Point> a,vector<cv::Point> b){
    return (cv::contourArea(a)>cv::contourArea(b));
}


int countWrinkle(cv::Mat & src,cv::Mat &mark, int r,int c,int num,int &area,int & len){
    if((int)src.at<uchar>(r,c)>200 && (int)mark.at<uchar>(r,c)==0 && r>=0 && c>=0 &&r<src.rows-1 &&c<src.cols-1)
    {
        mark.at<uchar>(r,c)=num;
        int up=0,upleft=0,upright=0;
        
        if( countWrinkle(src,mark,r-1,c,num,area,len))
        {
            area++;len++;
            //rr--;
            up=1;
            
        }
        if(countWrinkle(src,mark,r-1,c-1,num,area,len))
        {
            area++;len++;
            //rr--;cc--;
            upleft=1;
        }
        
        if(countWrinkle(src,mark,r-1,c+1,num,area,len))
        {
            area++;len++;
            //rr--;cc++;
            upright=1;
        }
        if(upright==1 && up==1 && upleft==1)len-=2;
        else if (up==0 && upright==1&& upleft==1)len--;
        
        
        
        
        if(countWrinkle(src,mark,r,c-1,num,area,len))
        {
            area++;
            //cc--;
        }
        if( countWrinkle(src,mark,r,c+1,num,area,len))
        {
            area++;
            //cc++;
        }
        countWrinkle(src,mark,r+1,c-1,num,area,len);
        countWrinkle(src,mark,r+1,c,num,area,len);
        countWrinkle(src,mark,r+1,c+1,num,area,len);

        return 1;
    }
    return 0;
    
}
cv::Mat KWrinkleDetect::showWrinkleCommonLine(){
    cv::Mat src=matSrc.clone();
    cv::Mat mark=matTemp.clone();

    //****************************
    
    vector<vector<cv::Point>> contours_small;
    cv::threshold(mark, mark, 100, 255,CV_THRESH_BINARY );
    
    //filter
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::Mat mark2=mark.clone();
//    cv::morphologyEx(mark2,mark2,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
//    cv::imwrite(markedpath+"tttt0.jpg", mark2);
    
    
    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultMask = cv::Mat ::zeros(mark.size(),CV_8U);
    resultMask.setTo(0);
    cv::Mat resultImage = cv::Mat ::zeros(mark.size(),CV_8U);
    
    sort(contours.begin(),contours.end(),cmp);
    
    contours_small.clear();
    for(int i=0;i<contours.size();i++)
    {
        vector<vector<cv::Point>>::iterator it=contours.begin()+i;
        cv::Point2f center; float radius;
        minEnclosingCircle(contours[i],center,radius);
        float cirArea=3.1415926*radius*radius;
        float div=contourArea(contours[i])/cirArea;
        
        if(cv::contourArea(contours[i])<minlimit || div>0.15)
        {
            contours.erase(it);
            i--;
        }
        
    }
    
    contours_small.clear();
    for(int i=0;i<contours.size();i++)
    {
//        if(i>60)break;
        contours_small.push_back(contours[i]);
        
        
    }
    
    drawContours(resultMask, contours_small, -1, cv::Scalar(255),CV_FILLED);
    //    cv::threshold(resultMask, resultMask, 0, 255, CV_THRESH_BINARY);
    cv::Mat resultImage1;
    
//    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/wrinkle_resultImagemark.bmp", mark);

    mark.copyTo(resultImage, resultMask);
    
//    cv::morphologyEx(resultImage,resultImage,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
//    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/wrinkle_resultImage.bmp", resultImage);
    
//    mark2=resultImage.clone();
//    contours.clear();
//    hierarchy.clear();
//    contours_small.clear();
//    resultMask.setTo(0);
//    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
//    sort(contours.begin(),contours.end(),cmp);
//    for(int i=0;i<contours.size();i++)
//    {
////        if(i>60)break;
//        contours_small.push_back(contours[i]);
//        
//        
//    }
//    drawContours(resultMask, contours_small, -1, cv::Scalar(255),CV_FILLED);
//    resultImage.copyTo(resultImage1, resultMask);
//    
//    
//    
//    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/wrinkle_resultImage1.bmp", resultImage1);
    
    
    
    
    //*********************************
//    cv::Mat mark3(resultImage.size(),CV_8UC1);
//    mark3.setTo(0);
//    vector<int> ans_area;
//    vector<float> ans_len;
    
//    int num=0;
//    for(int r=resultImage.rows-1;r>=0;r--)
//        for(int c=0;c<resultImage.cols;c++)
//        {
//            if((int)resultImage1.at<uchar>(r,c)>250 && (int)mark3.at<uchar>(r,c)==0)
//            {
//                num++;
//                int area=0,len=0;
//                int rr=r,cc=c;
//                countWrinkle(resultImage1, mark3, r, c, num,area,len);
//                //float tlen=sqrt((rr-r)*(rr-r)+(cc-c)*(cc-c));
//                ans_len.push_back(len);
//                ans_area.push_back(area);
//                
//            }
//            
//        }//for
//    
//    sort(ans_len.begin(),ans_len.end());
//    sort(ans_area.begin(),ans_area.end());
//    
//    for(int i=0;i<ans_len.size();i++){
//        cout<<"第"<<i<<"根皱纹： 长度"<<ans_len[i]<<"(像素)-----  面积："<<ans_area[i]<<"像素。"<<endl;
//        
//    }
    
    
    
    
    
    //begin to draw
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            //            cout<<(int)mark.at<uchar>(r,c)<<"--";
            if((int)resultImage.at<uchar>(r,c)>225){
                src.at<cv::Vec3b>(r,c)[0]=5;
                src.at<cv::Vec3b>(r,c)[1]=255;
                src.at<cv::Vec3b>(r,c)[2]=10;
                
                
            }
            
        }
    
    return src;
    
}
