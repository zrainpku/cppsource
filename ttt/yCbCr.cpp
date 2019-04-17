////
////  yCbCr.cpp
////  ttt
////
////  Created by 朱瑞 on 17/8/9.
////  Copyright © 2017年 朱瑞. All rights reserved.
////
//
//#include "yCbCr.hpp"
//#include <opencv2/opencv.hpp>
//
//void skinDetection(){
//  
//cv::namedWindow("input image");
//cv::namedWindow("output mask");
//cv::namedWindow("output image");
///*椭圆皮肤模型*/
//cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);
//ellipse(skinCrCbHist, cv::Point(109, 152.6), cv::Size(25.39, 14.2), 145.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);
//
//
//cv::Mat input_image;
//input_image=cv::imread("/Users/zrain/Desktop/pic/eyelash_source/eye31.bmp");
//cv::Mat output_mask;
//cv::Mat output_image;
//cv:: Mat ycrcb_image;
//output_mask = cv::Mat::zeros(input_image.size(), CV_8UC1);
//cv::cvtColor(input_image, ycrcb_image, CV_BGR2YCrCb); //首先转换成到YCrCb空间
//for(int i = 0; i < input_image.cols; i++)   //利用椭圆皮肤模型进行皮肤检测
//for(int j = 0; j < input_image.rows; j++){
//    cv::Vec3b ycrcb = ycrcb_image.at<cv::Vec3b>(j, i);
//    if(skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0)
//        output_mask.at<uchar>(j, i) = 255;
//        }
//input_image.copyTo(output_image, output_mask);
//
//imshow("input image", input_image);
//imshow("output mask", output_mask);
//imshow("output image", output_image);
//cv::waitKey();
//
//}
