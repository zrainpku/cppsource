//
//  wrinkle_detection.cpp
//  ttt
//
//  Created by 朱瑞 on 17/7/10.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "wrinkle_detection.hpp"
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
#include "wavelet.hpp"

using namespace cv;
using namespace std;




void gradientGray(Mat &src, Mat &mag)
{
    const int H = src.rows, W = src.cols;
    Mat Ix(H, W, CV_32S), Iy(H, W, CV_32S);
    //因为计算出的梯度值可能有正有负，且值也可能会很大，故数据类型为整形
    
    // 求水平方向梯度，处理左右边缘像素
    for (int y = 0; y < H; y++){
        Ix.at<int>(y, 0) = abs(src.at<uchar>(y, 1) - src.at<uchar>(y, 0)) * 2;
        for (int x = 1; x < W - 1; x++)
            Ix.at<int>(y, x) = abs(src.at<uchar>(y, x + 1) - src.at<uchar>(y, x - 1));
        Ix.at<int>(y, W - 1) = abs(src.at<uchar>(y, W - 1) - src.at<uchar>(y, W - 2))*2;
    }
    // 求垂直方向梯度，处理左右边缘像素
    for (int x = 0; x < W; x++)  {
        Iy.at<int>(0, x) = abs(src.at<uchar>(1, x) - src.at<uchar>(0, x)) * 2;
        for (int y = 1; y < H - 1; y++)
            Iy.at<int>(y, x) = abs(src.at<uchar>(y + 1, x) - src.at<uchar>(y - 1, x));
        Iy.at<int>(H - 1, x) = abs(src.at<uchar>(H - 1, x) - src.at<uchar>(H - 2, x)) * 2;
    }
    /*for (int j = 0; j < H; j++)
     for (int k = 0; k < W; k++)
     {
     mag.at<byte>(j, k) = min(Ix.at<int>(j,k) + Iy.at<int>(j, k), 255);
     }*/
    convertScaleAbs(min(Ix + Iy, 255), mag); //这句话和上面的for循环是同样的功能
}



void WrinkleDetection::imgToRidge(string read_path, string write_path){
    
    cv::Mat matSrc=cv::imread(read_path,CV_LOAD_IMAGE_UNCHANGED);
    cv::cvtColor(matSrc, matSrc, CV_BGR2GRAY);
    cv::Mat matDst(matSrc.size(),CV_8U);
//    vector<cv::Mat> labimg;
//    cv::split(matSrc, labimg);
//    matSrc=labimg[0];
    
    
//    matDst=HighPass(matSrc);
    int threshold = 0;
    float amount =5;
    cv::Mat imgblurred;
    cv::GaussianBlur(matSrc, imgblurred, cv::Size(61, 61), 0, 0);
    cv::Mat lowcontrastmask = abs(matSrc - imgblurred)<threshold;
    matDst = matSrc*(1 + amount) + imgblurred*(-amount);
    matSrc.copyTo(matDst, lowcontrastmask);
    
    float alpha = 1.6;
    float beta = -20;
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

    cv::imwrite(write_path+"wrinkle_temp_gray.bmp", matDst);

    //end gray
    
    
    int scale = 1;
    int delta = 0;
    if( !matDst.data )
    {
        cout<<"load gray image error!"<<endl;
        return ;
    }

    Mat grad,grad_temp;;
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;


    Sobel( matDst, grad_x, matDst.depth(), 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    
    Sobel( matDst, grad_y, matDst.depth(), 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );


    grad.setTo(0);
    cv::Mat check=cv::abs(abs_grad_x)<cv::abs(abs_grad_y);
    abs_grad_x.copyTo(grad);
    abs_grad_y.copyTo(grad,check);

    
    cv::imwrite(write_path+"wrinkle_temp_abs.bmp", grad);
    cv::imwrite(write_path+"wrinkle_temp_abs_x.bmp", abs_grad_x);
    cv::imwrite(write_path+"wrinkle_temp_abs_y.bmp", abs_grad_y);

    //end grident
    
    frangi2d_opts_t opt;
    opt.sigma_start=3;
    opt.sigma_end=5;
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
//    cv::Mat src_x(grad.size(),CV_32FC1);
//    abs_grad_x.convertTo(src_x, CV_32FC1);
//    cv::Mat outfrangi_x(grad.size(),CV_32FC1);
//    frangi2d(src_x,outfrangi_x,scales,directions,opt);
    
//    cv::Mat src_y(grad.size(),CV_32FC1);
//    abs_grad_y.convertTo(src_y, CV_32FC1);
//    cv::Mat outfrangi_y(grad.size(),CV_32FC1);
//    frangi2d(src_y,outfrangi_y,scales,directions,opt);
    //end frangi
    cv::Mat out,out_x(grad.size(),CV_8U),out_y(grad.size(),CV_8U);
//    outfrangi.convertTo(outfrangi, CV_8UC1);
//    outfrangi_x.convertTo(outfrangi_x, CV_8U);
//    outfrangi_y.convertTo(outfrangi_y, CV_8U);

    cv::threshold(outfrangi, out, 0, 255,CV_THRESH_BINARY);
//    cv::threshold(out, out, 200, 255,CV_THRESH_BINARY_INV);
//    cv::medianBlur(out, out, 3);


//    cv::medianBlur(out, out, 3);
    cv::imwrite(write_path+"wrinkle_ridge.bmp", out);
//    cv::imwrite(write_path+"wrinkle_ridge_x.bmp", out_x);
//    cv::imwrite(write_path+"wrinkle_ridge_y.bmp", out_y);



}


void WrinkleDetection::showWrinkleCommonInit(string init,string markedpath,string outpath){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(markedpath+"wrinkle_ridge.bmp",CV_LOAD_IMAGE_GRAYSCALE);

    
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            if(r<0)
                cout<<(int)mark.at<uchar>(r,c)<<",";
            if((int)mark.at<uchar>(r,c)>200 ){
                src.at<Vec3b>(r,c)[0]=200;
                src.at<Vec3b>(r,c)[1]=50;
                src.at<Vec3b>(r,c)[2]=20;
                
                
            }
            
        }
    cv::imwrite(outpath, src);
    
}


bool lessToGreater(vector<cv::Point> a,vector<cv::Point> b){
    return (cv::contourArea(a)>cv::contourArea(b));
}


int findWrinkle(cv::Mat & src,cv::Mat &mark, int r,int c,int num,int &area,int & len){
    if((int)src.at<uchar>(r,c)>200 && (int)mark.at<uchar>(r,c)<num && r>=0 && c>=0 &&r<src.rows-1 &&c<src.cols-1)
    {
        mark.at<uchar>(r,c)=num;
        int up=0,upleft=0,upright=0;
        
        if( findWrinkle(src,mark,r-1,c,num,area,len))
        {
            area++;len++;
            //rr--;
            up=1;
            
        }
        if(findWrinkle(src,mark,r-1,c-1,num,area,len))
        {
            area++;len++;
            //rr--;cc--;
            upleft=1;
        }
        
        if(findWrinkle(src,mark,r-1,c+1,num,area,len))
        {
            area++;len++;
            //rr--;cc++;
            upright=1;
        }
        if(upright==1 && up==1 && upleft==1)len-=2;
        else if (up==0 && upright==1&& upleft==1)len--;
        
        
        
        
        if(findWrinkle(src,mark,r,c-1,num,area,len))
        {
            area++;
            //cc--;
        }
        if( findWrinkle(src,mark,r,c+1,num,area,len))
        {
            area++;
            //cc++;
        }
        return 1;
    }
    return 0;
    
}
void WrinkleDetection::showWrinkleCommonLine(int minlimit,int maxlimit, string init,string markedpath,string outpath){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(markedpath+"wrinkle_ridge.bmp",CV_LOAD_IMAGE_GRAYSCALE);
//    cv::Mat mark_x=cv::imread(markedpath+"wrinkle_ridge_x.bmp",CV_LOAD_IMAGE_GRAYSCALE);
//    cv::Mat mark_y=cv::imread(markedpath+"wrinkle_ridge_y.bmp",CV_LOAD_IMAGE_GRAYSCALE);


//****************************

    vector<vector<Point>> contours_small;
    cv::threshold(mark, mark, 100, 255,CV_THRESH_BINARY );
    
    //filter
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    cv::Mat mark2=mark.clone();
    cv::morphologyEx(mark2,mark2,MORPH_OPEN,getStructuringElement(MORPH_RECT, Size(3, 3)));
    cv::imwrite(markedpath+"tttt0.jpg", mark2);

    
    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    Mat resultMask = Mat ::zeros(mark.size(),CV_8U);
        resultMask.setTo(0);
    Mat resultImage = Mat ::zeros(mark.size(),CV_8U);

    sort(contours.begin(),contours.end(),lessToGreater);

    contours_small.clear();
    for(int i=0;i<contours.size();i++)
    {
        vector<vector<Point>>::iterator it=contours.begin()+i;
        cv::Point2f center; float radius;
        minEnclosingCircle(contours[i],center,radius);
        float cirArea=3.1415926*radius*radius;
        float div=contourArea(contours[i])/cirArea;

        if(cv::contourArea(contours[i])<minlimit || div>0.1)
        {
            contours.erase(it);
            i--;
        }
        
    }

        contours_small.clear();
        for(int i=0;i<contours.size();i++)
        {
            if(i>50)break;
            contours_small.push_back(contours[i]);
            
            
        }
  
    drawContours(resultMask, contours_small, -1, Scalar(255),CV_FILLED);
//    cv::threshold(resultMask, resultMask, 0, 255, CV_THRESH_BINARY);
    cv::Mat resultImage1,resultImage2;
    
        mark.copyTo(resultImage2, resultMask);
       mark.copyTo(resultImage, resultMask);

    cv::morphologyEx(resultImage,resultImage,MORPH_OPEN,getStructuringElement(MORPH_RECT, Size(3, 3)));
    cv::imwrite(markedpath+"tttt.jpg", resultImage);

//        changeToframe(resultImage);
//        cv::morphologyEx(resultImage,resultImage,MORPH_DILATE,getStructuringElement(MORPH_RECT, Size(5,5)));
//        cv::morphologyEx(resultImage,resultImage,MORPH_OPEN,getStructuringElement(MORPH_RECT, Size(3, 3)));

    mark2=resultImage.clone();
    contours.clear();
    hierarchy.clear();
    contours_small.clear();
    resultMask.setTo(0);
    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    sort(contours.begin(),contours.end(),lessToGreater);
    for(int i=0;i<contours.size();i++)
    {
        if(i>50)break;
        contours_small.push_back(contours[i]);
        
        
    }
    drawContours(resultMask, contours_small, -1, Scalar(255),CV_FILLED);
    resultImage.copyTo(resultImage1, resultMask);



    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/resultImage1.jpg", resultImage1);
    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/resultImage2.jpg", resultImage2);



  
   //*********************************
    cv::Mat mark3(resultImage.size(),CV_8UC1);
    mark3.setTo(0);
    vector<int> ans_area;
    vector<float> ans_len;
    
    int num=0;
    
    for(int r=resultImage.rows-1;r>=0;r--)
        for(int c=0;c<resultImage.cols;c++)
        {
            if((int)resultImage1.at<uchar>(r,c)>250 && (int)mark3.at<uchar>(r,c)==0)
            {
                num++;
                int area=0,len=0;
                int rr=r,cc=c;
                                findWrinkle(resultImage2, mark3, r, c, num,area,len);
                //                float tlen=sqrt((rr-r)*(rr-r)+(cc-c)*(cc-c));
                                ans_len.push_back(len);
                                ans_area.push_back(area);
                
            }
            
        }//for
    
    sort(ans_len.begin(),ans_len.end());
    sort(ans_area.begin(),ans_area.end());
    
    for(int i=0;i<ans_len.size();i++){
        cout<<"第"<<i<<"根皱纹： 长度"<<ans_len[i]<<"(像素)-----  面积："<<ans_area[i]<<"像素。"<<endl;
        
    }
    
    
    
  
    
    //begin to draw
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            //            cout<<(int)mark.at<uchar>(r,c)<<"--";
            if((int)resultImage1.at<uchar>(r,c)>225){
                src.at<Vec3b>(r,c)[0]=5;
                src.at<Vec3b>(r,c)[1]=255;
                src.at<Vec3b>(r,c)[2]=10;
                
                
            }
            
        }

    cv::imwrite(outpath, src);
    
}


string WrinkleDetection::imgToGray(string filename,string outputpath) {

    cv::Mat matSrc=cv::imread(filename,CV_LOAD_IMAGE_UNCHANGED);
    cv::cvtColor(matSrc, matSrc, CV_BGR2Lab);
    vector<cv::Mat> labimg;
    cv::split(matSrc, labimg);
    cv::Mat matDst(matSrc.size(),CV_8U);

    
    matSrc=labimg[0];
    int threshold = 0;
    float amount = 0;
    cv::Mat imgblurred;
    cv::GaussianBlur(matSrc, imgblurred, cv::Size(71, 71), 0, 0);
    cv::Mat lowcontrastmask = abs(matSrc - imgblurred)<threshold;
    matDst = matSrc*(1 + amount) + imgblurred*(-amount);
    matSrc.copyTo(matDst, lowcontrastmask);

    float alpha = 1.6;
    float beta = -10;
    for (int y = 0; y < matDst.rows; y++)
    {
        for (int x = 0; x < matDst.cols; x++)
        {
            matDst.at<uchar>(y, x)= cv::saturate_cast<uchar>(alpha*(matDst.at<uchar>(y, x)) + beta);
        }
    }
  
    cv::equalizeHist(matDst, matDst);
    
    cv::Mat diamond = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(matDst, matDst, cv::MORPH_OPEN, diamond);
    
    

    
    
//####
    

    
    
    imwrite(outputpath, matDst);
    return outputpath;
    
}








string WrinkleDetection::grayToGrident(string filename,string outputpath){
    /// 装载图像
    Mat src=imread(filename,CV_LOAD_IMAGE_UNCHANGED);
    Mat grad;
    string write_path = outputpath;
    int scale = 1;
    int delta = 0;
//    int ddepth = CV_16S;
    int ddepth = src.depth();
    
    
    if( !src.data )
    {
        cout<<"load gray image error!"<<endl;
        return NULL;
    }
    
    
  
    
    /// 创建 grad_x 和 grad_y 矩阵
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    
    /// 求 X方向梯度
    //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    Sobel( src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    
    /// 求Y方向梯度
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    Sobel( src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    
    /// 合并梯度(近似)
//    addWeighted( abs_grad_x, 0.4, abs_grad_y, 0.6, 0, grad );
    
    cv::Mat check=cv::abs(abs_grad_x)<cv::abs(abs_grad_y);
    abs_grad_x.copyTo(grad);
    abs_grad_y.copyTo(grad,check);
    
    Mat temp;
    grad_x.convertTo(abs_grad_x, CV_32FC1);
    grad_y.convertTo(abs_grad_y, CV_32FC1);
//    cartToPolar(abs_grad_x, abs_grad_y, grad, temp);


    
    imwrite(write_path, grad);


    
    return write_path;
}









string WrinkleDetection::gridenToFrangi(string filename,string outputpath){
   
    frangi2d_opts_t opt;
    opt.sigma_start=3;
    opt.sigma_end=11;
    opt.sigma_step=2;
    opt.BetaOne=0.5;
    opt.BetaTwo=15;
    opt.BlackWhite=true;
    
    cv::Mat imggrident=imread(filename,CV_LOAD_IMAGE_UNCHANGED);
    
    cv::Mat scales(imggrident.size(),CV_32FC1);
    cv::Mat directions(imggrident.size(),CV_32FC1);
    cv::Mat src(imggrident.size(),CV_32FC1);
    
    imggrident.convertTo(src, CV_32FC1);
    cv::Mat outfrangi(imggrident.size(),CV_32FC1);
    
    frangi2d(src,outfrangi,scales,directions,opt);
    cout<<outfrangi.type()<<endl;
    imwrite(outputpath, outfrangi);
    
    return  outputpath;
    
    
}




string WrinkleDetection::frangiToRidge(string filename,string outputpath){
    Mat src=imread(filename,CV_LOAD_IMAGE_UNCHANGED);
    string write_path=outputpath;
    Mat dest(src.size(),CV_8U);
    cout<<src.type();
    cv::threshold(src, dest, 0.0, 255,CV_THRESH_BINARY );
    
 
    imwrite(write_path, dest);
    return write_path;
    
}


string WrinkleDetection::ridgeToThreshold(string filename,string outputpath){
    Mat src=imread(filename,CV_LOAD_IMAGE_UNCHANGED);
    string write_path=outputpath;
    //##############
    // 查找轮廓，对应连通域
    vector<vector<cv::Point>> contours ;
    cv::findContours(src,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    
    // 寻找最大连通域
    double maxArea = 0;
    vector<cv::Point> maxContour;
    for(size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea)
        {
            maxArea = area;
            maxContour = contours[i];
        }
    }
    
    // 将轮廓转为矩形框
    //cv::Rect maxRect = cv::boundingRect(maxContour);
    
    // 显示连通域
    cv::Mat result1, result2;
    
    src.copyTo(result1);
    src.copyTo(result2);
    
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Rect r = cv::boundingRect(contours[i]);
        cv::rectangle(result1, r, cv::Scalar(255));
    }

    
    
    
    imwrite(write_path, result1);
    return write_path;
    
}




void WrinkleDetection::showWrinkleInit(string init,string marked,string outpath){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(marked,CV_LOAD_IMAGE_GRAYSCALE);


    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            if(r<0)
            cout<<(int)mark.at<uchar>(r,c)<<",";
            if((int)mark.at<uchar>(r,c)>200){
                src.at<Vec3b>(r,c)[0]=200;
                src.at<Vec3b>(r,c)[1]=50;
                src.at<Vec3b>(r,c)[2]=20;

                
            }
        
        }
    cv::imwrite(outpath, src);

}



void WrinkleDetection::showWrinkleLine(int minlimit,int maxlimit,string init,string marked,string outpath){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(marked,CV_LOAD_IMAGE_UNCHANGED);
    
    //cv::morphologyEx(mark,mark,MORPH_B,getStructuringElement(MORPH_RECT, Size(3, 3)));
    //cv::bilateralFilter_8u(mark, mark, 25, 25 * 2, 25 / 2);
    cv::Mat elem(5,5,CV_8U);
    elem.setTo(0);
    elem.at<uchar>(0,4)=1;
    elem.at<uchar>(1,3)=1;
    elem.at<uchar>(2,2)=1;
    elem.at<uchar>(3,1)=1;
    elem.at<uchar>(4,0)=1;
    
//    cv::Mat elem(3,3,CV_8U);
//    elem.setTo(0);
//    elem.at<uchar>(0,2)=1;
//    elem.at<uchar>(1,1)=1;
//    elem.at<uchar>(2,0)=1;

//    cv::morphologyEx(mark,mark,MORPH_OPEN,getStructuringElement(MORPH_RECT, Size(3, 3)));
//    cv::morphologyEx(mark,mark,MORPH_OPEN,elem);
    cv::threshold(mark, mark, 100, 255,CV_THRESH_BINARY );

    //filter
    vector<vector<Point>> contours;
    vector<vector<Point>> contours_small;
    vector<Vec4i> hierarchy;
    cv::Mat mark2=mark.clone();
//    changeToframe(mark2);
//     cv::morphologyEx(mark2,mark2,MORPH_OPEN,elem);
//    cv::morphologyEx(mark2, mark2, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(3,3)));
    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    Mat resultMask = Mat ::zeros(mark.size(),CV_8U);
    Mat resultImage = Mat ::zeros(mark.size(),CV_8U);

    for(int i=0;i<contours.size();i++)
    {
        
        if(cv::contourArea(contours[i])>minlimit )
            contours_small.push_back(contours[i]);
 
    }

    
    drawContours(resultMask, contours_small, -1, Scalar(255),CV_FILLED);
    cv::threshold(resultMask, resultMask, 0, 255, CV_THRESH_BINARY);

    mark.copyTo(resultImage, resultMask);



    //begin to draw
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
//            cout<<(int)mark.at<uchar>(r,c)<<"--";
            if((int)resultImage.at<uchar>(r,c)>155){
                src.at<Vec3b>(r,c)[0]=0;
                src.at<Vec3b>(r,c)[1]=255;
                src.at<Vec3b>(r,c)[2]=0;
                
                
            }
            
        }
    
    
    cv::Mat mark3(resultImage.size(),CV_8UC1);
    mark3.setTo(0);
    vector<int> ans_area;
    vector<float> ans_len;
    
    int num=0;
    
    for(int r=resultImage.rows-1;r>=0;r--)
        for(int c=0;c<resultImage.cols;c++)
        {
            if((int)resultImage.at<uchar>(r,c)>250 && (int)mark3.at<uchar>(r,c)==0)
            {
                num++;
                int area=0,len=0;
                //int rr=r,cc=c;
//                findWrinkle(resultImage2, mark3, r, c, num,area,len);
                //                float tlen=sqrt((rr-r)*(rr-r)+(cc-c)*(cc-c));
//                ans_len.push_back(len);
//                ans_area.push_back(area);
     
            }
            
        }//for
    
    sort(ans_len.begin(),ans_len.end());
    sort(ans_area.begin(),ans_area.end());
    
    for(int i=0;i<ans_len.size();i++){
        cout<<"第"<<i<<"根皱纹： 长度"<<ans_len[i]<<"(像素)-----  面积："<<ans_area[i]<<"像素。"<<endl;
        
    }
    
    
    cv::imwrite(outpath, src);
    
}

void WrinkleDetection::drawWrinkleBlack(int minlimit,int maxlimit, string init,string gray,string out){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(gray,CV_LOAD_IMAGE_UNCHANGED);
    cv::morphologyEx(mark,mark,MORPH_ERODE,getStructuringElement(MORPH_RECT, Size(3, 3)));
    cv::morphologyEx(mark,mark,MORPH_DILATE,getStructuringElement(MORPH_RECT, Size(3, 3)));

//    imwrite("/Users/zrain/Desktop/pic/wrinkle_result/initline.jpg", mark);
    vector<vector<Point>> contours;
    vector<vector<Point>> contours_out;
    vector<Vec4i> hierarchy;
    findContours(mark, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    Mat resultImage = Mat ::zeros(mark.size(),CV_8U);
    for(int i=0;i<contours.size();i++)
    {
        //if(cv::contourArea(contours[i])>minlimit && cv::contourArea(contours[i])<maxlimit)
            contours_out.push_back(contours[i]);
    
    }
    drawContours(resultImage, contours_out, -1, Scalar(255));
    cv::imwrite(out, resultImage);


}

void WrinkleDetection::drawWrinkleRaw(int minlimit,int maxlimit,string init,string gray,string out){
    cv::Mat src=cv::imread(init,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat mark=cv::imread(gray,CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat elem(5,5,CV_8U);
    elem.setTo(0);
    elem.at<uchar>(0,4)=1;
    elem.at<uchar>(1,3)=1;
    elem.at<uchar>(2,2)=1;
    elem.at<uchar>(3,1)=1;
    elem.at<uchar>(4,0)=1;
    
    cv::morphologyEx(mark,mark,MORPH_OPEN,getStructuringElement(MORPH_RECT, Size(3, 3)));
    cv::threshold(mark, mark, 100, 255, CV_THRESH_BINARY);

    vector<vector<Point>> contours;
    vector<vector<Point>> contours_out;
    vector<Vec4i> hierarchy;
    findContours(mark, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    Mat resultMask = Mat ::zeros(mark.size(),CV_8U);
    Mat resultImage = Mat ::zeros(mark.size(),CV_8U);

    for(int i=0;i<contours.size();i++)
    {
        if(cv::contourArea(contours[i])>minlimit)
            contours_out.push_back(contours[i]);
        
    }
    drawContours(resultMask, contours_out, -1, Scalar(255));
    
//    cv::morphologyEx(resultImage,resultImage,MORPH_OPEN,elem);
    for(int r=0;r<mark.rows;r++)
        for(int c=0;c<mark.cols;c++)
        {
            if(resultMask.at<uchar>(r,c)==255)
            {
                src.at<Vec3b>(r,c)[0]=255;
                src.at<Vec3b>(r,c)[1]=50;
                src.at<Vec3b>(r,c)[2]=20;
            }
        }
    cv::imwrite(out, src);
    
    
}






