//
//  crop.cpp
//  ttt
//
//  Created by 朱瑞 on 17/7/10.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "crop.hpp"
#include<strstream>
#include <iostream>
#include<opencv2/opencv.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include "dlib/opencv.h"
#include<vector>
#include <string>




using namespace std;
using namespace cv;

string model_path="/Users/zrain/dlib/dlib/shape_predictor_68_face_landmarks.dat";


int cropImage(string filename, int point, Rect rec1,Rect rec2,Rect rec3) {
    // insert code here...
    cv::Mat img=cv::imread(filename);
    Mat mask,img1;
    Rect r1=rec1;
    Rect r2=rec2;
    Rect r3=rec3;
    mask = Mat::zeros(img.size(),CV_8UC1);
    mask(r1).setTo(255);
    mask(r2).setTo(255);
    mask(r3).setTo(255);
    img.copyTo(img1,mask);
    
    imwrite("/Users/zrain/Desktop/pic/wrinkle.jpg", img1);
    
    return 0;
}

int cropImageROI(string filename,string outpath) {
    // insert code here...
    cv::Mat img=cv::imread(filename);
    Mat mask,imageROI;
//    cvSetImageROI(img, rect);
    imageROI= img(cv::Rect(900,2500,1600,1000));
    // load the mask (must be gray-level)
  

    imwrite(outpath, imageROI);
    
    return 0;
}


void cropAcne(string fileRgb,string fileGray,string outpath){
    cv::Mat src=cv::imread(fileRgb);
    cv::Mat mark=cv::imread(fileGray,CV_8UC1);
    
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::Mat mark2=mark.clone();
    
    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultMask =cv:: Mat ::zeros(mark2.size(),CV_8U);
    cv::Mat resultImage = cv::Mat ::zeros(mark2.size(),CV_8U);
    vector<vector<cv::Point>> contours_small;
    contours_small.clear();
    vector<Rect> boundRect(contours.size());
    for(int i=0;i<contours.size();i++)
    {
    
        cv::Rect rec= cv::boundingRect(contours[i]);
        int W=rec.width;
        int H=rec.height;
        //heitou 10 10 20 20
        //baitou 30 30 60 60
        //nongbao 30 60
        int topx=max(0,rec.x-30);
        int topy=max(0,rec.y-30);
        int width_rec=W+60;
        if((rec.x+width_rec)>mark2.cols)
            width_rec=W;
        int heigth_rec=H+60;
        if((rec.y+heigth_rec)>mark2.rows)
            heigth_rec=H;
        
        cv::Mat temp=src(cv::Rect(topx,topy,width_rec,heigth_rec));
        stringstream ss;
        string str;
        ss<<i;
        ss>>str;
        string str1;
        str1="022r";
        cv::imwrite(outpath+str1+str+".jpg", temp);
        


        
    }
    
}



int markImagePoints(string filename,string outpath) {
    // insert code here...
    cv::Mat img=cv::imread(filename);
    
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    dlib::shape_predictor sp;
    dlib::deserialize(model_path) >> sp;
    //Wavelet wavelet(NLAYER); //
    Mat resized_img,img_ans;
    resized_img=img.clone();
    
    
    dlib::cv_image<dlib::bgr_pixel> dlib_img(resized_img);
    // Now tell the face detector to give us a list of bounding boxes around all the faces in the image.
    std::vector<dlib::rectangle> faces = detector(dlib_img);
    std::cout << "Number of faces detected: " << faces.size() << std::endl;
    // Now we will go ask the shape_predictor to tell us the pose of each face we detected.
    std::vector<dlib::full_object_detection> shapes;
    dlib::full_object_detection shape;
 
    for (int j = 0; j < faces.size(); ++j)
    {
        shape= sp(dlib_img, faces[j]);

        shapes.push_back(shape);
        for (unsigned long i = 0; i < shape.num_parts(); i++)
        {
          
            Point land_point;
            land_point.x = (int)shape.part(i).x();
            land_point.y = (int)shape.part(i).y();
            cv::circle(resized_img, land_point, 3, Scalar(255-i*3, 255-i*3, 255-i*3));
            std::strstream ss; string s; ss << i; ss >> s;
            cv::putText(resized_img, s, land_point, 1, 0.8f, Scalar(255, 0, 0));
        }
        //cv::rectangle(cap_img, Point(shape.part(1).x(), shape.part(1).y()), Point(shape.part(48).x(), shape.part(48).y()), Scalar(255, 255, 255));


        imwrite(outpath, resized_img);
        
        
    }
    
    cout<<"succed marked image!"<<endl;

    
    return 0;
}


vector<Point> getImagePoint(string filename) {
    // insert code here...
    cv::Mat img=cv::imread(filename);
    
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    dlib::shape_predictor sp;
    dlib::deserialize(model_path) >> sp;
    //Wavelet wavelet(NLAYER); //
    Mat resized_img,img_ans;
    resized_img=img.clone();
    
    
    dlib::cv_image<dlib::bgr_pixel> dlib_img(resized_img);
    // Now tell the face detector to give us a list of bounding boxes around all the faces in the image.
    std::vector<dlib::rectangle> faces = detector(dlib_img);
    std::cout << "Number of faces detected: " << faces.size() << std::endl;
    // Now we will go ask the shape_predictor to tell us the pose of each face we detected.
    std::vector<dlib::full_object_detection> shapes;
    dlib::full_object_detection shape;
    vector<Point> out_point;
    if(faces.size()==0){cout<<"none face detected!"<<endl;return out_point;}
    for (int j = 0; j < faces.size(); ++j)
    {
        shape= sp(dlib_img, faces[j]);
        
        shapes.push_back(shape);
        for (unsigned long i = 0; i < shape.num_parts(); i++)
        {
            Point p;
            p.x=(int)shape.part(i).x();
            p.y=(int)shape.part(i).y();
            out_point.push_back(p);

        }
        
        
    }
    
    cout<<"succeed get points !"<<endl;
    
    return out_point;
}



#define NUM_STEPS 30 //越大，曲线越密，越逼近
void curve4(vector<cv::Point> &p,
            double x1, double y1,   //Anchor1
            double x2, double y2,   //Control1
            double x3, double y3,   //Control2
            double x4, double y4)   //Anchor2
{
    cv::Point tmp0(x1, y1);
    p.push_back(tmp0);
    double dx1 = x2 - x1;
    double dy1 = y2 - y1;
    double dx2 = x3 - x2;
    double dy2 = y3 - y2;
    double dx3 = x4 - x3;
    double dy3 = y4 - y3;
    
    double subdiv_step = 1.0 / (NUM_STEPS + 1);
    double subdiv_step2 = subdiv_step*subdiv_step;
    double subdiv_step3 = subdiv_step*subdiv_step*subdiv_step;
    
    double pre1 = 3.0 * subdiv_step;
    double pre2 = 3.0 * subdiv_step2;
    double pre4 = 6.0 * subdiv_step2;
    double pre5 = 6.0 * subdiv_step3;
    
    double tmp1x = x1 - x2 * 2.0 + x3;
    double tmp1y = y1 - y2 * 2.0 + y3;
    
    double tmp2x = (x2 - x3)*3.0 - x1 + x4;
    double tmp2y = (y2 - y3)*3.0 - y1 + y4;
    
    double fx = x1;
    double fy = y1;
    
    double dfx = (x2 - x1)*pre1 + tmp1x*pre2 + tmp2x*subdiv_step3;
    double dfy = (y2 - y1)*pre1 + tmp1y*pre2 + tmp2y*subdiv_step3;
    
    double ddfx = tmp1x*pre4 + tmp2x*pre5;
    double ddfy = tmp1y*pre4 + tmp2y*pre5;
    
    double dddfx = tmp2x*pre5;
    double dddfy = tmp2y*pre5;
    
    int step = NUM_STEPS;
    
    while (step--)
    {
        fx += dfx;
        fy += dfy;
        dfx += ddfx;
        dfy += ddfy;
        ddfx += dddfx;
        ddfy += dddfy;
        cv::Point tmp1(fx, fy);
        p.push_back(tmp1);
    }
    cv::Point tmp2(x4, y4);
    p.push_back(tmp2);
}

void drawR33Eye(cv::Mat &roi, vector<cv::Point> points)
{
//    //1
    cv::Point p1 = cv::Point((points[39].x + points[27].x) / 2,  points[27].y);
    cv::Point p2 = cv::Point(points[40].x , points[29].y+100  );
    cv::Point p3 = cv::Point(points[36].x , points[36].y+350);
    //2
    cv::Point p4 = cv::Point(points[17].x ,  (points[27].y+points[22].y)/2);
    int height5=points[27].y+points[22].y-500;
    int weight5=points[17].x + points[4].x-100;
    cv::Point p5 = cv::Point( weight5/ 2,  height5/2);
    cv::Point p6 = cv::Point((points[3].x + points[4].x) / 2,  points[31].y);
    //cv::Point p6 = cv::Point(points[4].x , points[29].y);
    //3
    cv::Point p7 = cv::Point((points[5].x+points[6].x)/2 , points[33].y);
    cv::Point p8 = cv::Point((points[7].x+points[6].x)/2 , points[33].y);
    cv::Point p9 = cv::Point(points[31].x,  (points[29].y+points[30].y)/2);



    vector<cv::Point> curvePoint;
    curve4(curvePoint,p1.x, p1.y,p2.x, p2.y,p3.x, p3.y,p4.x, p4.y);
    curve4(curvePoint,p4.x, p4.y,p5.x, p5.y,p6.x, p6.y,p7.x, p7.y);
    curve4(curvePoint,p7.x, p7.y,p8.x, p8.y,p9.x, p9.y,p1.x, p1.y);


    cv::Scalar singleColor;
    if (roi.channels() == 3)singleColor = cv::Scalar(255, 255, 255);
    else singleColor = cv::Scalar(255);
    cout<<"begin to draw line!"<<endl;
    for (int i = 0; i < curvePoint.size() - 1; i++)
        cv::line(roi, curvePoint[i], curvePoint[i + 1], singleColor, 2, 8);
    
    int left=(points[3].x + points[4].x) / 2;
    int top=std::max(height5/2,points[27].y);
    int right=(points[39].x + points[27].x) / 2;
    int down=points[33].y;
    cv::Rect rec(left,top,right-left,down-top);
//
//    roi(rec).copyTo(roi);
    
    cv::floodFill(roi, cv::Point(1500,1000), cv::Scalar(0, 0, 0),&rec,cv::Scalar(50,50,50),cv::Scalar(50,50,50));
    for(int r=0;r<roi.rows;r++)
        for(int c=0;c<roi.cols;c++)
        {
            int sumelem=roi.at<Vec3b>(r,c)[0]+roi.at<Vec3b>(r,c)[1]+roi.at<Vec3b>(r,c)[2];
            if(sumelem==765){
                roi.at<Vec3b>(r,c)[0]=0;
                roi.at<Vec3b>(r,c)[1]=0;
                roi.at<Vec3b>(r,c)[2]=0;
            }
            
        }
//    cv::multiply(roi, tempimg/255, roi);
    




}

void drawL33Eye(cv::Mat roi, vector<cv::Point> points)
{
    //1
    cv::Point p1 = cv::Point((points[42].x + points[27].x) / 2,  points[27].y);
    cv::Point p2 = cv::Point((points[42].x + points[35].x) / 2, points[46].y );
    cv::Point p3 = cv::Point(points[47].x , (points[47].y + points[28].y) / 2);
    //2
    cv::Point p4 = cv::Point(points[46].x , (points[46].y + points[28].y) / 2);
    
    cv::Point p5 = cv::Point(points[25].x , (points[45].y + points[46].y) / 2);
    cv::Point p6 = cv::Point(points[26].x , points[44].y );
    //3
    cv::Point p7 = cv::Point((points[14].x + points[26].x) / 2, (points[26].y + points[44].y) / 2);
    
    cv::Point p8 = cv::Point((points[16].x + points[26].x) / 2, points[16].y);
    cv::Point p9 = cv::Point((points[14].x + points[26].x) / 2, points[15].y );
    //4
    cv::Point p10 = cv::Point(points[26].x, points[15].y);
    cv::Point p11 = cv::Point(points[25].x, (points[14].y+points[15].y)/2);
    cv::Point p12 = cv::Point(points[46].x, points[29].y);
    //5
    cv::Point p13 = cv::Point((points[46].x + points[47].x)/2, points[29].y);
    cv::Point p14 = cv::Point(points[23].x, (points[29].y+points[28].y)/2);
    cv::Point p15 = cv::Point(points[22].x, points[28].y);
    
    

    vector<cv::Point> curvePoint;
    curve4(curvePoint,p1.x, p1.y,p2.x, p2.y,p3.x, p3.y,p4.x, p4.y);
    curve4(curvePoint,p4.x, p4.y,p5.x, p5.y,p6.x, p6.y,p7.x, p7.y);
    curve4(curvePoint,p7.x, p7.y,p8.x, p8.y,p9.x, p9.y,p10.x, p10.y);
    curve4(curvePoint,p10.x, p10.y,p11.x, p11.y,p12.x, p12.y,p13.x, p13.y);
    curve4(curvePoint,p13.x, p13.y,p14.x, p14.y,p15.x, p15.y,p1.x, p1.y);
    
    cv::Scalar singleColor;
    if (roi.channels() == 3)singleColor = cv::Scalar(255, 255, 255);
    else singleColor = cv::Scalar(255);
    cout<<"begin to draw line!"<<endl;
    for (int i = 0; i < curvePoint.size() - 1; i++)
        cv::line(roi, curvePoint[i], curvePoint[i + 1], singleColor, 2, 8);
}





void drawLips(cv::Mat &roi, vector<cv::Point> points)
{
    //    //1
    cv::Point p1 = cv::Point(points[48].x ,  points[48].y);
    cv::Point p2 = cv::Point(points[49].x , points[49].y  );
    cv::Point p3 = cv::Point(points[50].x , points[50].y);
    cv::Point p4 = cv::Point(points[51].x ,  points[51].y);
    cv::Point p5 = cv::Point(points[52].x ,  points[52].y);
    cv::Point p6 = cv::Point(points[53].x , points[53].y  );
    cv::Point p7 = cv::Point(points[54].x , points[54].y);
    cv::Point p8 = cv::Point(points[55].x ,  points[55].y);
    cv::Point p9 = cv::Point(points[56].x ,  points[56].y);
    cv::Point p10 = cv::Point(points[57].x , points[57].y  );
    cv::Point p11 = cv::Point(points[58].x , points[58].y);
    cv::Point p12 = cv::Point(points[59].x ,  points[59].y);

    
    
    
    vector<cv::Point> curvePoint;
    curve4(curvePoint,p1.x, p1.y,p2.x, p2.y,p3.x, p3.y,p4.x, p4.y);
    curve4(curvePoint,p4.x, p4.y,p5.x, p5.y,p6.x, p6.y,p7.x, p7.y);
    curve4(curvePoint,p7.x, p7.y,p8.x, p8.y,p9.x, p9.y,p10.x, p10.y);
    curve4(curvePoint,p10.x, p10.y,p11.x, p11.y,p12.x, p12.y,p1.x, p1.y);
    
    
    cv::Scalar singleColor;
    if (roi.channels() == 3)singleColor = cv::Scalar(255, 255, 255);
    else singleColor = cv::Scalar(255);
    cout<<"begin to draw line!"<<endl;
    for (int i = 0; i < curvePoint.size() - 1; i++)
        cv::line(roi, curvePoint[i], curvePoint[i + 1], singleColor, 2, 8);
    

    
}
