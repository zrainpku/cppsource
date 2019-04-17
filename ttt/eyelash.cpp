//
//  eyelash.cpp
//  ttt
//
//  Created by 朱瑞 on 17/7/31.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "eyelash.hpp"
//#include "frangi_eyelash.hpp"
//#include "frame.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <strstream>
#include "crop.hpp"

#include <opencv/cv.h>
using namespace std;


KEyelashDetection::KEyelashDetection(cv::Mat mat){
    matSrc=mat.clone();
    matEyeLine=findEyeLine(mat);
    matTemp=imgProcess();
    matAns=showEyelashCommonInit();
    
    
}
cv::Mat KEyelashDetection::KGetEyelash()
{
    return matAns;
}


KEyelashDetection::~KEyelashDetection(){

}


//#######################################################
bool lessToGreater(cv::Point a,cv::Point b){
    return (a.x<b.x);
}


void changeConnectToBlack(cv::Mat &img,int r,int c){
    if(img.at<uchar>(r,c)==255)
    {
        img.at<uchar>(r,c)=0;
        changeConnectToBlack(img,r+1,c);
        changeConnectToBlack(img,r-1,c);
        changeConnectToBlack(img,r,c+1);
        changeConnectToBlack(img,r,c-1);

    }
}


cv:: Mat findEyeLine2(cv::Mat matsrc){
    cv::Mat src=matsrc.clone();
    cv::Mat eyeline(matsrc.size(),CV_8UC1);
    cv::Mat outimg=src.clone();

    
    
    //mask
    
    
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::Mat mark2=outimg.clone();
    
    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultMask =cv:: Mat ::zeros(mark2.size(),CV_8U);
    cv::Mat resultImage = cv::Mat ::zeros(mark2.size(),CV_8U);
    vector<vector<cv::Point>> contours_small;
    contours_small.clear();
    for(int i=0;i<contours.size();i++)
    {
        if(cv::contourArea(contours[i])>10 )
            contours_small.push_back(contours[i]);
        
    }
    drawContours(resultMask, contours_small, -1, cv::Scalar(255),CV_FILLED);
    cv::Mat out;
    outimg.copyTo(out, resultMask);
    //    cv::morphologyEx(out, out, cv::MORPH_ERODE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)));
    cv::imwrite("/Users/zrain/Desktop/pic/eyelash_mid/eye_maoutimg.jpg", out);
    //    cv::imwrite("/Users/zrain/Desktop/pic/eyelash_mid/eye_mout.jpg", out);
    //    cv::imwrite("/Users/zrain/Desktop/pic/eyelash_mid/eye_mast_resultmask.jpg", resultMask);
    cv::Mat mask=out.clone();
    
    
    //find top left top right
    
    
    
    
    
    vector<int> mark(2000);
    for(int i=0;i<mask.cols;i++)
        mark.push_back(0);
    
    vector<cv::Point> eyeLine;
    int radius=10;
    int heightest=0;
    int count=0;
    int flags=0;
    for(int c=radius;c<mask.cols-radius;c++)
        for(int r=radius;r<mask.rows-radius;r++)
        {
            for(int i=c;i<c+radius;i++)
            {
                flags=0;
            
            if(mask.at<uchar>(r,i)==255 && mark[c]==0)
            {
                count++;
//                if(count<3)changeConnectToBlack(mask, r, i);
                flags=1;
                if(heightest==0)heightest=r;
                mark[c]=1;
                if(abs(r-heightest)<75){
                    cv::Point pot=cv::Point(i,r);
                    eyeLine.push_back(pot);
                }
             
                break;
            }//if
                
           }//for
            if(flags==1)break;
    
        }//for
    //    int heightest=0;
    //    for(int r=0;r<mask.rows;r++)
    //    {
    //        //         int first_row=0;// row r first point
    //        for(int c=0;c<mask.cols;c++)
    //        {
    //
    //            if(mask.at<uchar>(r,c)==255 &&mark[c]==0)
    //            {
    //                mark[c]=1;
    //                if(heightest==0)heightest=r;
    //
    //                if((r-heightest)<90){
    ////                    changeConnectToBlack(mask, r, c);
    //
    //                    cv::Point pot=cv::Point(c,r);
    //                    eyeLine.push_back(pot);
    //                }
    //
    //
    //
    //            }
    //
    //        }
    //    }//for
    //    cv::imwrite("/Users/zrain/Desktop/pic/eyelash_mid/eye_mast_resultmask.jpg", mask);
    
    sort(eyeLine.begin(), eyeLine.end(), lessToGreater);
        eyeLine.pop_back();
        eyeLine.pop_back();
    
    long step=eyeLine.size()/13;
    long i=0;
    //    cv::Point p1 = cv::Point(eyeLine[i+2].x,  eyeLine[i+2].y);
    cv::Point p1 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    
    i+=step;
    cv::Point p2 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p3 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p4 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p5 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p6 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p7 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p8 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p9 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p10 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p11 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p12 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    
    i=eyeLine.size()-1;
    cv::Point p13 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    
    
    
    vector<cv::Point> curvePoint;
    curve4(curvePoint,p1.x, p1.y,p2.x, p2.y,p3.x, p3.y,p4.x, p4.y);
    curve4(curvePoint,p4.x, p4.y,p5.x, p5.y,p6.x, p6.y,p7.x, p7.y);
    curve4(curvePoint,p7.x, p7.y,p8.x, p8.y,p9.x, p9.y,p10.x, p10.y);
    curve4(curvePoint,p10.x, p10.y,p11.x, p11.y,p12.x, p12.y,p13.x, p13.y);
    
    
    cv::Scalar singleColor;
    if (mask.channels() == 3)singleColor = cv::Scalar(255, 255, 255);
    else singleColor = cv::Scalar(255);
    cout<<"begin to draw eye line!"<<endl;
    eyeline.setTo(0);
    for (int i = 0; i < curvePoint.size() - 1; i++)
        cv::line(eyeline, curvePoint[i], curvePoint[i + 1], singleColor, 2, 8);
    
    //    cv::imwrite("/Users/zrain/Desktop/pic/eyelash_mid/eye_eyeline_11.jpg", eyeline);
    
    return eyeline;

   
}

//#####################################
cv:: Mat KEyelashDetection::findEyeLine(cv::Mat matsrc){
    cv::Mat matSrc1=matsrc.clone();
    
    cv::Mat matLab;
    cv::cvtColor(matSrc1, matLab, CV_BGR2Lab);
    vector<cv::Mat> rgbchannels;
    cv::split(matSrc1, rgbchannels);
    cv::Mat matgray_lab(matSrc1.size(),CV_8UC1);
    
    matgray_lab=rgbchannels[2].clone();
    matSrc1=matgray_lab.clone();



    
        cv::Mat outimg(matSrc1.size(),CV_8UC1);
        outimg.setTo(0);
//61 11 ,16
    cv::GaussianBlur(matSrc1, matSrc1, cv::Size(61,11), 16);
//    cv::imwrite("/Users/zrain/Desktop/pic/eyelash_mid/eye_mask_ycxxxxxrcb.jpg", matSrc);
//106
    cv::threshold(matSrc1, outimg, 106, 255, cv::THRESH_BINARY_INV);

  
    
    //mask
    cv::morphologyEx(outimg, outimg, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)));
    cv::imwrite("/Users/zrain/Desktop/pic/eyelash_mid/eye_mask_ycrcb.jpg", outimg);
    
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::Mat mark2=outimg.clone();
    
    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultMask =cv:: Mat ::zeros(mark2.size(),CV_8U);
    cv::Mat resultImage = cv::Mat ::zeros(mark2.size(),CV_8U);
    vector<vector<cv::Point>> contours_small;
    contours_small.clear();
    for(int i=0;i<contours.size();i++)
    {
        if(cv::contourArea(contours[i])>100 )
            contours_small.push_back(contours[i]);
        
    }
    drawContours(resultMask, contours_small, -1, cv::Scalar(255),CV_FILLED);
    cv::Mat out;
    outimg.copyTo(out, resultMask);
    cv::Mat mask=out.clone();

    
    //find top left top right

    
    

    
    vector<int> mark(2000);
    for(int i=0;i<mask.cols;i++)
        mark.push_back(0);
    
    vector<cv::Point> eyeLine;
    int radius=10;
    int heightest=0;
    int count=0;
    for(int c=radius;c<mask.cols-radius;c++)
        for(int r=radius;r<mask.rows-radius;r++)
        {
            
            
                if(mask.at<uchar>(r,c)==255 && mark[c]==0)
                {
                
//                    count++;
//                    if(count<10)
//                    changeConnectToBlack(mask,r,c);
                    if(heightest==0)heightest=r;
                    mark[c]=1;
                    if(abs(r-heightest)<110){
                    cv::Point pot=cv::Point(c,r);
                    eyeLine.push_back(pot);
                    }
                
                }
            
        
        }


    sort(eyeLine.begin(), eyeLine.end(), lessToGreater);
//    eyeLine.pop_back();
//    eyeLine.pop_back();
    
    long step=eyeLine.size()/13;
    long i=0;
//    cv::Point p1 = cv::Point(eyeLine[i+2].x,  eyeLine[i+2].y);
    cv::Point p1 = cv::Point(eyeLine[i].x,  eyeLine[i].y);

    i+=step;
    cv::Point p2 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p3 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p4 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p5 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p6 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p7 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p8 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p9 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p10 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p11 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    i+=step;
    cv::Point p12 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    
    i=eyeLine.size()-1;
    cv::Point p13 = cv::Point(eyeLine[i].x,  eyeLine[i].y);
    
    
    
    vector<cv::Point> curvePoint;
    curve4(curvePoint,p1.x, p1.y,p2.x, p2.y,p3.x, p3.y,p4.x, p4.y);
    curve4(curvePoint,p4.x, p4.y,p5.x, p5.y,p6.x, p6.y,p7.x, p7.y);
    curve4(curvePoint,p7.x, p7.y,p8.x, p8.y,p9.x, p9.y,p10.x, p10.y);
    curve4(curvePoint,p10.x, p10.y,p11.x, p11.y,p12.x, p12.y,p13.x, p13.y);
    
    
    cv::Scalar singleColor;
    if (mask.channels() == 3)singleColor = cv::Scalar(255, 255, 255);
    else singleColor = cv::Scalar(255);
    cout<<"begin to draw eye line!"<<endl;
    cv::Mat eyeline(mask.size(),CV_8UC1);
    eyeline.setTo(0);
    for (int i = 0; i < curvePoint.size() - 1; i++)
        cv::line(eyeline, curvePoint[i], curvePoint[i + 1], singleColor, 2, 8);
    
    
    cv::threshold(eyeline, eyeline, 50, 255, cv::THRESH_BINARY);
    cv::morphologyEx(eyeline, eyeline, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7)));

    return eyeline;
}

void findConnected(cv::Mat & img,cv::Mat &outimg,cv::Mat &mark, int r,int c){
    if(img.at<uchar>(r,c)>=199 && mark.at<uchar>(r,c)==0 && r>0 && r<img.rows-1 && c>0 && c<img.cols-1){
        if(img.at<uchar>(r,c)>=250)
        outimg.at<uchar>(r,c)=255;
        mark.at<uchar>(r,c)=255;
        
        findConnected(img, outimg, mark, r+1, c-1);
        findConnected(img, outimg, mark, r+1, c);
        findConnected(img, outimg, mark, r+1, c+1);
        
        findConnected(img, outimg, mark, r-1, c-1);
        findConnected(img, outimg, mark, r-1, c);
        findConnected(img, outimg, mark, r-1, c+1);
        findConnected(img, outimg, mark, r, c-1);
        findConnected(img, outimg, mark, r, c+1);
    
    }
    return;
}

cv::Mat findeyeRect(cv::Mat eyeline0,cv::Mat eyelashsrc0){
    //change eyeline to black
    cv::Mat eyeline=eyeline0.clone();
    cv:: Mat eyelashsrc=eyelashsrc0.clone();
    cv::threshold(eyeline, eyeline, 50, 255, cv::THRESH_BINARY_INV);
    cv::threshold(eyelashsrc, eyelashsrc, 50, 255, cv::THRESH_BINARY);
    cv::Mat eyelash(eyeline.size(),CV_8UC1);
    eyelash.setTo(0);
    vector<int> mark(2000);
    for(int i=0;i<eyeline.cols;i++)
        mark.push_back(0);
        
    int min_c=0,max_c=100;
    for(int c=0;c<eyeline.cols;c++)
        for(int r=0;r<eyeline.rows;r++)
        {
            int maxlen=min(220, eyeline.rows-r);

            if(eyeline.at<uchar>(r,c)==0 && mark[c]==0)
            {
                min_c=min(min_c, c);
                max_c=max(max_c, c);
                mark[c]=1;
                for(int rec=r;rec<maxlen+r;rec++)
                {
                    eyelash.at<uchar>(rec,c)=255;
                }
            
            }//if
            else if(mark[c]==1)
            {
                break;
            }//else
        }
    
  
    
    
    
    
    return eyelash;
}


cv::Mat  KEyelashDetection::imgProcess(){
    
    cv::Mat src=matSrc.clone();
    
    cv::Mat matLab;
    cv::cvtColor(src, matLab, CV_BGR2Lab);
    vector<cv::Mat> rgbchannels;
    cv::split(src, rgbchannels);
    cv::Mat matgray_lab(src.size(),CV_8UC1);
    
    matgray_lab=rgbchannels[2].clone();
//    cv::cvtColor(matLab, matSrc, CV_BGR2GRAY);

    cv::Mat eyelashsrc;
    cv::adaptiveThreshold(matgray_lab, eyelashsrc, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15,5);
//    cv::adaptiveThreshold(matgray_lab, tempmask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 15, 5);
//    cv::imwrite(write_path+"eye_tempmask1.jpg", tempmask);
    cv::threshold(eyelashsrc, eyelashsrc, 50, 255, cv::THRESH_BINARY_INV);
    cv::Mat imgcanny;
    cv::Canny(matgray_lab, imgcanny, 100, 150);

    
    cv::Mat grad_x;
    


//    cv::imwrite(write_path+"eye_rgb_r.jpg", matgray_lab);

    cv::Sobel(matgray_lab, grad_x, matgray_lab.depth(), 1, 0);
    cv::threshold(grad_x, grad_x, 10, 255, cv::THRESH_BINARY);

//    cv::imwrite(write_path+"eye_grad_x.jpg", grad_x);

    
//    cv::Mat eyeline=findEyeLine(matgray_lab);
    cv::Mat eyeline=matEyeLine.clone();


//    cv::imwrite(write_path+"eyeline.bmp", eyeline);

    cv::Mat eyelashsrc2(eyelashsrc.size(),CV_8UC1);
    eyelashsrc2.setTo(0);

    int r=0,c=0;
    cv::Mat mark(eyelashsrc.size(),CV_8UC1);
    mark.setTo(0);
    cv::Mat outimg(eyelashsrc.size(),CV_8UC1);
    cv::Mat eyeline2(eyelashsrc.size(),CV_8UC1);
    eyeline2.setTo(0);

    outimg.setTo(0);
//    eyelashsrc2=eyelashsrc |eyeline;
//    cv::imwrite(write_path+"eye_eyelash2.jpg", eyelashsrc2);

    
    for( int i=0;i<mark.cols;i++)
    {
        for(int j=0;j<mark.rows;j++)
        {
            if(eyeline.at<uchar>(j,i)==255 && r==0){r=j+55;c=i;}
            
            if(eyeline.at<uchar>(j,i)==255 &&eyelashsrc.at<uchar>(j+55,i)!=255)
            {
                eyeline2.at<uchar>(j+55,i)=199;
                
            }
            
        }
    }
    
    eyelashsrc2=eyelashsrc | eyeline2;
//    cv::imwrite(write_path+"eye_connect1.bmp", eyelashsrc2);

    
    findConnected(eyelashsrc2, outimg, mark, r, c);
//    cv::imwrite(write_path+"eye_connect.bmp", outimg);

    return outimg;



//    cv::imwrite(write_path+"eyelash.bmp", outimg);
    
    
    
    
    
}


int findEyelashRight(cv::Mat & src,cv::Mat &mark,cv::Mat &eyeline,int r,int  c,int num){

//    if(r<=0 || c<=0 || r>src.rows-1 || c>src.cols-1 )return 0;
//
//    if((int)eyeline.at<uchar>(r,c)==255) return 0;
  
        
    if(r>0 && c>0 && r<src.rows-1 && c<src.cols-1 && (int)src.at<uchar>(r,c)>250 && (int)mark.at<uchar>(r,c)<num && (int)eyeline.at<uchar>(r,c)==0)
    {
        mark.at<uchar>(r,c)=num;

        
        findEyelashRight(src,mark,eyeline,r-1,c,num);
        findEyelashRight(src,mark,eyeline,r-1,c-1,num);
        findEyelashRight(src,mark,eyeline,r-1,c+1,num);
        findEyelashRight(src,mark,eyeline,r,c-1,num);
        findEyelashRight(src,mark,eyeline,r,c+1,num);

     
        
        
      

      
        return 1;
    }
    return 0;

}

int findEyelashLeft(cv::Mat & src,cv::Mat &mark,cv::Mat & eyeline, int r,int  c,int num,int & len,int & area){
    if(r<=0 || c<=0 || r>src.rows-1 || c>src.cols-1 || (int)eyeline.at<uchar>(r,c)==255)return 0;

    if((int)src.at<uchar>(r,c)>250 && (int)mark.at<uchar>(r,c)<num )
    {
        mark.at<uchar>(r,c)=num;
        int up=0,upleft=0,upright=0;
        
        
        if( findEyelashLeft(src,mark,eyeline,r-1,c,num,len,area))
        {len++;up=1;area++;}
 
        if(findEyelashLeft(src,mark,eyeline,r-1,c-1,num,len,area))
        {len++;upleft=1;area++;}
        if(findEyelashLeft(src,mark,eyeline,r-1,c+1,num,len,area))
        {len++;upright=1;area++;}
        int ans=up+upleft+upright;
        if(ans==3) len-=2;
        else if(ans==2)len--;
        if(findEyelashLeft(src,mark,eyeline,r,c-1,num,len,area))
            area++;
        if(findEyelashLeft(src,mark,eyeline,r,c+1,num,len,area))
            area++;

            
            
       
        
        
        return 1;
    }
    return 0;
    
}

int findEyelashArea(cv::Mat & src,cv::Mat &mark,cv::Mat &eyeline, int r,int  c,int num,int & len,int & area){
//    if(r<=0 || c<=0 || r>src.rows-1 || c>src.cols-1 )return 0;
    
    if(r>0 && c>0 && r<src.rows-1 && c<src.cols-1 && (int)src.at<uchar>(r,c)>250 && (int)mark.at<uchar>(r,c)==0 && (int)eyeline.at<uchar>(r,c)==0)
    {
        mark.at<uchar>(r,c)=num;
        area++;
//        cout<<num<<"---"<<area<<endl;
        
        if( findEyelashArea(src,mark,eyeline,r-1,c,num,len,area))
        {
            area++;
            len++;
            
        }
        if(findEyelashArea(src,mark,eyeline,r-1,c-1,num,len,area))
            area++;
        if(findEyelashArea(src,mark,eyeline,r-1,c+1,num,len,area))
            area++;
//        if(findEyelashArea(src,mark,eyeline,r,c-1,num,len,area))
//            area++;
//        if(findEyelashArea(src,mark,eyeline,r,c+1,num,len,area))
//            area++;
        
        
        
        return 1;
    }
    return 0;
    
}


void createEyelashLabel(std::string init,std::string markedpath,std::string out){

    
    cv::Mat mask=cv::imread(markedpath+"eyelash.bmp",CV_8UC1);
    cv::Mat eyeline=cv::imread(markedpath+"eyeline.bmp",CV_8UC1);
    cv::threshold(mask, mask, 30, 255, CV_THRESH_BINARY);
    cv::threshold(eyeline, eyeline, 50, 255, CV_THRESH_BINARY);
    
    cv::Mat  src=cv::imread(init);
    
    
    
    for(int r=mask.rows-1;r>0;r--)
        for(int c=1;c<mask.cols;c++)
        {
        
            
            if((int)mask.at<uchar>(r,c)>250 ){
                src.at<uchar>(r,c)=1;

                
            }
            
            if((int)eyeline.at<uchar>(r,c)==255){
                src.at<uchar>(r,c)=2;

            }
            
            
            
            
        }//for
    cv::imwrite(out, src);



}





cv::Mat KEyelashDetection::showEyelashCommonInit(){
    
//    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
//    cv::Mat mask=cv::imread(markedpath+"eyelash.bmp",CV_8UC1);
//    cv::Mat eyeline=cv::imread(markedpath+"eyeline.bmp",CV_8UC1);
    cv::Mat src=matSrc.clone();
    cv::Mat mask=matTemp.clone();
    cv::Mat eyeline=matEyeLine.clone();
    
    cv::threshold(mask, mask, 30, 255, CV_THRESH_BINARY);
    cv::threshold(eyeline, eyeline, 50, 255, CV_THRESH_BINARY);

    
    cv::Mat mark1(mask.size(),CV_8UC1 );
    mark1.setTo(0);
    cv::Mat mark2(mask.size(),CV_8UC1 );
    mark2.setTo(0);
    cv::Mat mark3(mask.size(),CV_8UC1 );
    mark3.setTo(0);
    vector<int> ans_area;
    vector<float> ans_len;


    int num=0,maxeye=0;
    
    
    for(int row=0;row<mask.rows;row++)
    {
        for(int col=0;col<mask.cols;col++)
        {
        if(eyeline.at<uchar>(row,col)==255)
            maxeye=row;
        }
        if(maxeye>0)break;
    
    }
    
    for(int r=mask.rows-1;r>0;r--)
        for(int c=1;c<mask.cols;c++)
        {
            if((int)mask.at<uchar>(r,c)==255 && (int)mark1.at<uchar>(r,c)==0)
            {
                num++;
                int area=0,len=0,len_max=0;
                //int rr=r,cc=c;
//                findEyelashLeft(mask , mark1,eyeline, r, c, num,len_left,area);
//                findEyelashRight(mask , mark1,eyeline, r, c, num);
                findEyelashArea(mask , mark1 ,eyeline ,r, c, num,len,area);
                len_max=r-maxeye;
//                len=max(len, len_max);
              
                if(len>10){
                ans_len.push_back(len);
                ans_area.push_back(area);
                }
                
                
               
            }
            
            if((int)mask.at<uchar>(r,c)>250 ){
                src.at<cv::Vec3b>(r,c)[0]=0;
                src.at<cv::Vec3b>(r,c)[1]=255;
                src.at<cv::Vec3b>(r,c)[2]=0;

            }
            
            if((int)eyeline.at<uchar>(r,c)==255){
                src.at<cv::Vec3b>(r,c)[0]=0;
                src.at<cv::Vec3b>(r,c)[1]=0;
                src.at<cv::Vec3b>(r,c)[2]=255;
            }
            
            
            
            
        }//for
    
    sort(ans_len.begin(),ans_len.end());
    sort(ans_area.begin(),ans_area.end());
    strstream ss; string s; ss << num; ss >> s;
    string title=" total eyelash is: ";
    string title_len=" min and max length  is: ";

//    cv::putText(src, title, cv::Point(100,100), 1, 3.0f, cv::Scalar(255, 0, 0),4,8);
//    cv::putText(src, s, cv::Point(100,200), 1, 3.0f, cv::Scalar(255, 0, 0),4,8);
//    int maxlen=ans_len[ans_len.size()-1];
//    int minlen=ans_len[0];
//    strstream sslen; string slen;
//    sslen << maxlen<<"----"<<minlen;
//    sslen >> slen;
//    cv::putText(src, title_len, cv::Point(100,300), 1, 3.0f, cv::Scalar(255, 0, 0),4,8);
//    cv::putText(src, slen, cv::Point(100,400), 1, 3.0f, cv::Scalar(255, 0, 0),4,8);

    
    
    for(int i=0;i<ans_len.size();i++){
        cout<<"第"<<i<<"根睫毛： 长度"<<ans_len[i]<<"(像素)"<<"面积："<<ans_area[i]<<endl;
    
    }


//    cv::imwrite(out, src);
    return src;

}

