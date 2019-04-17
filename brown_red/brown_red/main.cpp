//
//  main.cpp
//  brown_red
//
//  Created by 朱瑞 on 17/7/13.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include <iostream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/ml/ml.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

float writeR=250;
float writeG=250;
float writeB=250;
string Path="/Users/zrain/Desktop/change/01LMJ/";
string BasePath="/Users/zrain/Desktop/pic/";


void showRGBenlment(cv::Mat &img1,cv::Mat &img2,cv::Rect rec){
    cv::Mat dest1=img1(rec);
    cv::Mat dest2=img2(rec);
    
    for(int r=0;r<dest1.rows;r++)
        for(int c=0;c<dest1.cols;c++)
        {
            cout<<"img1:"<<dest1.at<Vec3b>(r,c)<<"img2:"<<dest2.at<Vec3b>(r,c)<<endl;
        
        
        
        }




}

/*
void colorToRed2(cv::Mat &img)
{
    int i, j;
    Mat image=img.clone();
    Mat hsv = cvCreateImage(img.size(), 8, 3);
    
    cvtColor(image,hsv,CV_BGR2HSV);
    int width = hsv.cols;
    int height = hsv.rows;
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            //CvScalar s_hsv = cvGet2D(hsv, i, j);//获取像素点为（j, i）点的HSV的值
            
             //opencv 的H范围是0~180，红色的H范围大概是(0~8)∪(160,180)
             //S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
             //V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
            
            CvScalar s_hsv=hsv.at<Vec3b>(i,j);
//            CvScalar s;
            if (!((((s_hsv.val[0]>0)&&(s_hsv.val[0]<10))) || ((s_hsv.val[0]>160)&&(s_hsv.val[0]<180))))
            {
               
                //cvSet2D(&hsv, i ,j, s);
//                hsv.at<Vec3b>(i,j).val[0]=0;
                
                int s=hsv.at<Vec3b>(i,j).val[1];
                
                s*=0.5;
                if(s>255)
                    hsv.at<Vec3b>(i,j).val[1]=255;
                else hsv.at<Vec3b>(i,j).val[1]=s;
                
                
                
                int v=hsv.at<Vec3b>(i,j).val[2];
              
                v*=3.7;
                if(v>255)
                    hsv.at<Vec3b>(i,j).val[2]=255;
                else hsv.at<Vec3b>(i,j).val[2]=v;

                
            }
            else{
//                hsv.at<Vec3b>(i,j)[0]=50;

                int temp2=hsv.at<Vec3b>(i,j)[2]*1;
                
                if(temp2>255)
                    hsv.at<Vec3b>(i,j)[2]=255;
                else
                    hsv.at<Vec3b>(i,j)[2]=temp2;
                
                
                
                
                int ss=hsv.at<Vec3b>(i,j)[1]*1.3;

                if(ss>255)
                    hsv.at<Vec3b>(i,j)[1]=255;
                else
                    hsv.at<Vec3b>(i,j)[1]=ss;
            
//
//
//            
            }
       
            
            
        }
    cvtColor(hsv,hsv,CV_HSV2BGR);
    imwrite(Path+"myred6.jpg", hsv);
//    Mat outputImage = cvCreateMat( hsv.rows, hsv.cols, CV_8UC3 );
//    cvConvert(hsv, outputImage);
//    cvNamedWindow("filter");
//    imshow("filter", hsv);
//    waitKey(0);
}
*/

void colorToBrown2(cv::Mat &img)
{
    int i, j;
    Mat image=img.clone();
    Mat hsv = cvCreateImage(img.size(), 8, 3);
    
    cvtColor(image,hsv,CV_BGR2HSV);
    int width = hsv.cols;
    int height = hsv.rows;
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            //CvScalar s_hsv = cvGet2D(hsv, i, j);//获取像素点为（j, i）点的HSV的值
            /*
             opencv 的H范围是0~180，红色的H范围大概是(0~8)∪(160,180)
             S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
             V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
             */
            CvScalar s_hsv=hsv.at<Vec3b>(i,j);
            CvScalar s;
            if (!((((s_hsv.val[0]>11)&&(s_hsv.val[0]<30)))))
            {
                s.val[0] =0;
                s.val[1]=0;
                s.val[2]=0;
                //cvSet2D(&hsv, i ,j, s);
                hsv.at<Vec3b>(i,j)[0]=0;
                hsv.at<Vec3b>(i,j)[1]=0;
                hsv.at<Vec3b>(i,j)[2]=0;
                
            }
            else{
                hsv.at<Vec3b>(i,j)[1]=255;
                
                hsv.at<Vec3b>(i,j)[2]=255;
                
                
            }
            
            
            
        }
    imwrite(Path+"mybrown5.jpg", hsv);
    //    Mat outputImage = cvCreateMat( hsv.rows, hsv.cols, CV_8UC3 );
    //    cvConvert(hsv, outputImage);
    //    cvNamedWindow("filter");
    //    imshow("filter", hsv);
    //    waitKey(0);
}



void rgb2RedRbx(cv::Mat &image,string outpath) {
        int nr= image.rows;
        int nc= image.cols ;
        Mat ans_float=Mat(image.size(), CV_8UC3);
        for (int j=0; j<nr; j++) {
            for (int i=0; i<nc; i++) {
                
                int b=image.at<cv::Vec3b>(j,i)[0];
                int g=image.at<cv::Vec3b>(j,i)[1];
                int r=image.at<cv::Vec3b>(j,i)[2];
                int R,B,X;
                R=-r*1.4328+g*3.3769-1.9820*b+210.4212;
                B=-r*2.5135+g*5.9406-3.5089*b+168.6945;
                X=-2.4767*r+5.8416*g-3.4743*b+189.1942;
  
                R=(R>255?255:R);
                B=(B>255?255:B);
                X=(X>255?255:X);
                R=(R<0?0:R);
                B=(B<0?0:B);
                X=(X<0?0:X);

                image.at<cv::Vec3b>(j,i)[2]=R;
                image.at<cv::Vec3b>(j,i)[1]=B;
                image.at<cv::Vec3b>(j,i)[0]=X;
                
    
    
            }
        }
    
        imwrite(outpath, image);
}


void rgb2BrownRbx(cv::Mat &image,string outpath) {
    //        image*=1.0/255;
    int nr= image.rows; // number of rows
    int nc= image.cols ; // total number of elements per line
    Mat ans_float=Mat(image.size(), CV_8UC3);
    for (int j=0; j<nr; j++) {
        for (int i=0; i<nc; i++) {
            
            int b=image.at<cv::Vec3b>(j,i)[0];
            int g=image.at<cv::Vec3b>(j,i)[1];
            int r=image.at<cv::Vec3b>(j,i)[2];
            //to rbx
            int R,B,X;
            int total=r+g+b;
            
            R=-r*0.9769-g*0.4649+1.6978*b+244.4915;
            B=-r*1.7595-g*0.7800+2.9344*b+236.0887;
            X=-2.1655*r-0.6956*g+3.2223*b+225.0261;
            
            
            
            R=(R>255?255:R);
            B=(B>255?255:B);
            X=(X>255?255:X);
            R=(R<0?0:R);
            B=(B<0?0:B);
            X=(X<0?0:X);
            //to rgb
            //                r=R*17.5494+117.4081*B-133.5610*X;
            //                g=R*32.9007+234.2386*B-264.9738*X;
            //                b=R*41.9487+300.7848*B-340.2472*X;
            image.at<cv::Vec3b>(j,i)[2]=R;
            image.at<cv::Vec3b>(j,i)[1]=B;
            image.at<cv::Vec3b>(j,i)[0]=X;
            
            
            
        }
    }
    
    //        ans_float*=255.0;
    imwrite(outpath, image);
}

vector<Mat> showSeqratedChannels(vector<Mat> channels) {
    vector<Mat> seqaratedChannels;
    for (int i = 0; i < 3; i++) {
        Mat zero = Mat::zeros(channels[0].rows, channels[0].cols, channels[0].type());
        vector<Mat>aux;
        for (int j = 0; j < 3; j++) {
            if (j == i){ aux.push_back(channels[i]);}
            else
                aux.push_back(zero);
        }
        Mat chann;
        merge(aux, chann);             //chann为输出的容器
        seqaratedChannels.push_back(chann);
    }
    return seqaratedChannels;
}


void colorToRed(cv::Mat &image) {
    
    vector<cv::Mat> channels;
    cv::cvtColor(image, image, CV_BGR2Lab);
    cv::split(image,channels);

//    vector<Mat> separated = showSeqratedChannels(channels);
    cv::Mat lab=channels[0];
    
    
    
    cout<<"change done!"<<endl;
    
    imwrite(Path+"lab_2.jpg", channels[2]);
    imwrite(Path+"lab_1.jpg", channels[1]);
    imwrite(Path+"lab_0.jpg", channels[0]);

}

//void RGB2Lab(cv::Mat &image){
//    Mat ans_xyz;
//    image*=1.0/255;
//    cvtColor(image, ans_xyz, CV_RGB2XYZ);
//    int nr= image.rows; // number of rows
//    int nc= image.cols ; // total number of elements per line
//    Mat ans_float=Mat(nr, nc, CV_32FC3);
//    for (int j=0; j<nr; j++) {
//        float* data_xyz= ans_xyz.ptr<float>(j);
//        float* data_float= ans_float.ptr<float>(j);
//        for (int i=0; i<nc; i++) {
//            //red
//            float X=data_xyz[i]/0.95047;
//            float Y=data_xyz[i+nc]/1.0;
//            //cout<<data_xyz[i+nc*2]<<endl;
//            float Z=data_xyz[i+nc*2]/1.08883;
//            float FX = X > 0.008856f ? pow(X,1.0f/3.0f) : (7.787f * X +0.137931f);
//            float FY = Y > 0.008856f ? pow(Y,1.0f/3.0f) : (7.787f * Y +0.137931f);
//            float FZ = Z > 0.008856f ? pow(Z,1.0f/3.0f) : (7.787f * Z +0.137931f);
//
//            data_float[i] = Y > 0.008856f ? (116.0f * FY - 16.0f) : (903.3f * Y);
//            data_float[i+nc] = 500.f * (FX - FY);
//            data_float[i+nc*2] = 200.f * (FY - FZ);
//     
//            
//        }
//    }
//
//    ans_float*=255.0;
//    imwrite(Path+"my_lab.jpg", ans_float);
//
//}

int main() {
    // insert code here...
//    std::cout << "Hello, World!\n";

    string  RBXpath="/Users/zrain/Desktop/pic/rgb/";
    Mat cross_raw=imread(RBXpath+"temp.jpg",CV_LOAD_IMAGE_UNCHANGED);
//    Mat cross_red=imread(Path+"cross_red.jpg",CV_LOAD_IMAGE_UNCHANGED);
//    Mat cross_brown=imread(Path+"cross_brown.jpg",CV_LOAD_IMAGE_UNCHANGED);

//    int row=cross_raw.rows;
//    int col=cross_raw.cols;
//    cout<<"row="<<row<<endl;
//    cout<<"col="<<col<<endl;
//    cout<<"chanels="<<cross_raw.channels()<<endl;
//    cout<<"is continues:"<<cross_raw.isContinuous()<<endl;
//    cout<<" elem Size="<<cross_raw.elemSize()<<" elem Size red="<<cross_red.elemSize()<<endl;
//    cout<<" elem Size of channels="<<cross_raw.elemSize1()<<endl;
//    cout<<"ju zheng type is:"<<cross_raw.type()<<"red - type is:"<<cross_red.type()<<endl;
//    cout<<"depth="<<cross_raw.depth()<<endl;
    
//    Vec3b *raw_data200=cross_raw.ptr<Vec3b>(1000);
//    Vec3b *red_data200=cross_red.ptr<Vec3b>(1000);
//    Vec3b *brown_data200=cross_brown.ptr<Vec3b>(1000);
//
//    
//    Mat ans1;
//    cvtColor(cross_raw, ans1, CV_RGB);
//    Vec3b *ans1_data200=ans1.ptr<Vec3b>(1000);
//
//    for(int i=700;i<720;i++)
//    cout<<"raw:"<<raw_data200[i]<<" brown:"<<brown_data200[i]<<" ans1:"<<ans1_data200[i]<<" red:"<<red_data200[i]<<endl;
//    resize(ans1, ans1, Size(500,600));
//        namedWindow("HSV");
//        imshow("HSV", ans1);


//    RGB2Lab(cross_raw);
//    colorToRed(cross_raw);
//    cv::Rect rec(0,0,50,50);
//    
//    showRGBenlment(cross_raw,cross_red,rec);
    rgb2RedRbx(cross_raw,RBXpath+"out_redrrr.jpg");
//    rgb2BrownRbx(cross_raw,RBXpath+"out_brown.jpg");
//    colorToRed2(cross_raw);
//    colorToBrown2(cross_raw);

    //    resize(cross_raw, cross_raw, Size(500,600));
//     colorToRed0(cross_raw);
//    Mat ans, ans1,ans2;
//    vector<cv::Mat> channels;
//    resize(cross_raw, cross_raw, Size(500,700));
//    cvtColor(cross_raw, ans, CV_RGB2BGRA);
//    cv::split(ans, channels);
//    namedWindow("H");
//    imshow("H", channels[0]);
//    namedWindow("S");
//    imshow("S", channels[1]);
//    namedWindow("V");
//    imshow("V", channels[2]);
//    namedWindow("HSV");
//    imshow("HSV", ans);
//    cvtColor(cross_raw, ans1, CV_RGB2HLS);
//    namedWindow("CV_RGB2YCrCb");
//    imshow("CV_RGB2YCrCb", ans1);
//    cvtColor(cross_raw, ans2, CV_RGB2Lab);
//    namedWindow("CV_RGB2Lab");
//    imshow("CV_RGB2Lab", ans2);
    
//    waitKey(0);
    return 0;
}
