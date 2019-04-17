//
//  frame.cpp
//  ttt
//
//  Created by 朱瑞 on 17/7/26.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "frame.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
using namespace cv;
using namespace std;
bool check(const Mat &input)
{
    int cols = input.cols;
    int rows = input.rows;
    for (int i = 0; i < rows ; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (input.at<uchar>(i, j) != 0)
                return false;
        }
    }
    return true;
}

void init_mat(Mat &src)
{
    src = Mat(10, 5, CV_8U, Scalar(255));
    src.at<uchar>(0,1) = src.at<uchar>(0, 2) = src.at<uchar>(0, 3) = src.at<uchar>(0, 4) = 0;
    src.at<uchar>(1, 0) = src.at<uchar>(1, 3) = src.at<uchar>(1, 4) = 0;
    src.at<uchar>(2, 0) = src.at<uchar>(2, 3) = src.at<uchar>(2, 4) = 0;
    src.at<uchar>(3, 0) = src.at<uchar>(3, 4) = 0;
    src.at<uchar>(4, 0) = src.at<uchar>(4, 4) = 0;
}

void init_mat2(Mat &src)
{
    src = Mat(320, 240, CV_8U, Scalar(0));//y320, x240
    for (int y = 10; y < 230; y++)
    {
        for (int x = 110; x < 130; x++)
        {
            src.at<uchar>(y, x) = 255;
        }
    }
    
    for (int y = 140; y < 180; y++)
    {
        for (int x = 10; x < 230; x++)
        {
            src.at<uchar>(y, x) = 255;
        }
    }
}

int  changeToframe(cv::Mat & img)
{
    std::vector<Mat> S;
    Mat src, dst, dst2;
    
    //init_mat2(src);
    //init_mat2(dst);
    
    src = img.clone();
    dst = img.clone();
    threshold(src, src, 100, 255, THRESH_BINARY);
    threshold(dst, dst, 100, 255, THRESH_BINARY);
    int K = 0;
    Mat element(3, 3, CV_8U, Scalar(255));
    //if check(src) == true
    while (!check(dst))
    {
        erode(dst, dst, element);
        K++;
    }
    //cout << "K: " << K << endl;
    K = K - 1;
    //imshow("src", src);
    for (int k = 0; k <= K; k++)
    {
        erode(src, dst, element, Point(-1, -1), k);
        morphologyEx(dst, dst2, MORPH_OPEN, element);
        Mat tmp = dst - dst2;
        S.push_back(tmp);
    }
    
    
    Mat result = Mat(src.size(), CV_8UC1, Scalar(0));
    
    Mat tmp = Mat(src.size(), CV_8U, Scalar(0));
    tmp = S[0] | S[1];
    for (int i = 2; i < S.size(); i++)
    {
        tmp = tmp | S[i];
        //cout << "i: " << i << endl;
    }
    //imshow("tmp", tmp);
    img=tmp.clone();
    return 0;
}
