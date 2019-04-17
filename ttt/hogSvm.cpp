////
////  hogSvm.cpp
////  ttt
////
////  Created by 朱瑞 on 17/8/21.
////  Copyright © 2017年 朱瑞. All rights reserved.
////
//
//#include "hogSvm.hpp"
//
//#include <opencv2/opencv.hpp>
//
//
//#include <stdio.h>
//#include <string.h>
//#include <cctype>
//#include<iostream>
//#include <fstream>
//
//using namespace cv;
////using namespace cv::ml;
//using namespace std;
//
//void trainSvmModel(string inputpath)
//{
//int ImgWidht = 64;
//int ImgHeight = 64;
//vector<string> img_path;
//vector<int> img_catg;
//int nLine = 0;
//string buf;
//    ifstream svm_data_baitou(inputpath+"baitou/train.txt" );
//    ifstream svm_data_qiuzhen(inputpath+"qiuzhen/train.txt" );
//    ifstream svm_data_nongbao(inputpath+"nongbao/train.txt" );
////    ifstream svm_data(inputpath+"train.txt" );
//
//unsigned long n;
//
//while( svm_data_baitou )
//{
//    if( getline( svm_data_baitou, buf ) )
//    {
////        if( nLine <10 )
//        buf=inputpath+"baitou/"+buf;
//            img_catg.push_back(0);//图像类别
//            img_path.push_back( buf );//图像路径
//       
//        nLine ++;
//    }
//}
//    
//svm_data_baitou.close();//关闭文件
// //**********qiuzhen
//    
//    while( svm_data_qiuzhen )
//    {
//        if( getline( svm_data_qiuzhen, buf ) )
//        {
//            //三种类别，前10个，中间10个，最后10个
//            //        if( nLine <10 )
//            buf=inputpath+"qiuzhen/"+buf;
//            img_catg.push_back(1);//图像类别
//            img_path.push_back( buf );//图像路径
//            
//            nLine ++;
//        }
//    }
//    
//    svm_data_qiuzhen.close();//关闭文件
// //**********nongbao
//    while( svm_data_nongbao )
//    {
//        if( getline( svm_data_nongbao, buf ) )
//        {
//            //三种类别，前10个，中间10个，最后10个
//            //        if( nLine <10 )
//            buf=inputpath+"nongbao/"+buf;
//            img_catg.push_back(2);//图像类别
//            img_path.push_back( buf );//图像路径
//            
//            nLine ++;
//        }
//    }
//    
//    svm_data_nongbao.close();//关闭文件
//    
//    
//
//Mat data_mat, res_mat;
//int nImgNum = nLine;            //读入样本数量
////样本矩阵，nImgNum：行数代表样本的数量，每一行就是由一张图片计算得到HOG的特征向量，
//data_mat = Mat::zeros( nImgNum, 1764, CV_32FC1 ); //HOG特征的位数： 9*(16/8)*(16/8)*[(64-16)/8+1]*[(64-16)/8+1]=1764
////类型矩阵,存储每个样本的类型标志
//res_mat = Mat::zeros( nImgNum, 1, CV_32FC1 );
//
//Mat src;
//Mat trainImg = Mat::zeros(ImgHeight, ImgWidht, CV_8UC3);//需要分析的图片
//
//for( string::size_type i = 0; i != img_path.size(); i++ )
//{
//    src = imread(img_path[i].c_str(), CV_LOAD_IMAGE_UNCHANGED);
//    
//    cout<<" processing "<<img_path[i].c_str()<<endl;
//
//    resize(src, trainImg, cv::Size(ImgWidht,ImgHeight), 0, 0, INTER_CUBIC);
//    HOGDescriptor *hog=new HOGDescriptor(cvSize(ImgWidht,ImgHeight),cvSize(16,16),cvSize(8,8),cvSize(8,8), 9);  //构造HOG，具体意思见参考文章1,2
//    vector<float>descriptors;//结果数组
//    hog->compute(trainImg, descriptors, Size(1,1), Size(0,0)); //调用计算函数开始计算
//    if (i==0)
//    {
//        data_mat = Mat::zeros( nImgNum, descriptors.size(), CV_32FC1 ); //根据输入图片大小进行分配空间
//    }
//    cout<<"HOG dims: "<<descriptors.size()<<endl;
//    n=0;
//    for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)
//    {
//        data_mat.at<float>(i,n) = *iter;
//        n++;
//    }
//    //cout<<SVMtrainMat->rows<<endl;
//    res_mat.at<float>(i, 0) =  img_catg[i];
//    cout<<" end processing "<<img_path[i].c_str()<<" "<<img_catg[i]<<endl;
//    }
//    
//    CvSVM svm ;
//    CvSVMParams param;
//    CvTermCriteria criteria;
//    criteria = cvTermCriteria( CV_TERMCRIT_EPS, 1000, FLT_EPSILON );
//    param = CvSVMParams( CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria );
//    
//    /*
//     SVM种类：CvSVM::C_SVC
//     Kernel的种类：CvSVM::RBF
//     degree：10.0（此次不使用）
//     gamma：8.0
//     coef0：1.0（此次不使用）
//     C：10.0
//     nu：0.5（此次不使用）
//     p：0.1（此次不使用）
//     然后对训练数据正规化处理，并放在CvMat型的数组里。
//     */
//    //☆☆☆☆☆☆☆☆☆(5)SVM学习☆☆☆☆☆☆☆☆☆☆☆☆
//    svm.train( data_mat, res_mat, Mat(), Mat(), param );
//    //☆☆利用训练数据和确定的学习参数,进行SVM学习☆☆☆☆
//    string savepath=inputpath+"SVM_DATA.xml";
//    const char * c_s=savepath.c_str();
//    svm.save(c_s);
//    
//    //检测样本
//
//    return ;
//}
//
//
//
//
//void testSvmModel(string inputpath){
//    
//    
//    int ImgWidht = 64;
//    int ImgHeight = 64;
//    string buf;
//    unsigned long n;
//    
//    //检测样本
//    vector<string> img_tst_path;
//    ifstream img_tst_baitou(inputpath + "qiuzhen/train.txt" );
//    while( img_tst_baitou )
//    {
//        if( getline( img_tst_baitou, buf ) )
//        {
//            buf=inputpath+"qiuzhen/"+buf;
//            
//            img_tst_path.push_back( buf );
//        }
//    }
//    img_tst_baitou.close();
//    
////    ifstream img_tst_nongbao(inputpath + "testnongbao/test.txt" );
////    while( img_tst_nongbao )
////    {
////        if( getline( img_tst_nongbao, buf ) )
////        {
////            buf=inputpath+"testnongbao/"+buf;
////            
////            img_tst_path.push_back( buf );
////        }
////    }
////    img_tst_nongbao.close();
//    
////    ifstream img_tst_qiuzhen(inputpath + "testqiuzhen/test.txt" );
////    while( img_tst_qiuzhen )
////    {
////        if( getline( img_tst_qiuzhen, buf ) )
////        {
////            buf=inputpath+"testqiuzhen/"+buf;
////            
////            img_tst_path.push_back( buf );
////        }
////    }
////    img_tst_qiuzhen.close();
//    
//    //#####end of test data###########
//    
//    Mat trainImg = Mat::zeros(ImgHeight, ImgWidht, CV_8UC3);//需要分析的图片
//    Mat test;
//    char line[512];
//    ofstream predict_txt(inputpath + "SVM_PREDICT.txt" );
//    for( string::size_type j = 0; j != img_tst_path.size(); j++ )
//    {
//        test = imread( img_tst_path[j].c_str(), 1);//读入图像
//        resize(test, trainImg, cv::Size(ImgWidht,ImgHeight), 0, 0, INTER_CUBIC);//要搞成同样的大小才可以检测到
//        HOGDescriptor *hog=new HOGDescriptor(cvSize(ImgWidht,ImgHeight),cvSize(16,16),cvSize(8,8),cvSize(8,8),9);  //窗口大小，块大小，块滑动增量，cell的大小，bins的个数
//        vector<float>descriptors;//结果数组
//        hog->compute(trainImg, descriptors,Size(1,1), Size(0,0)); //调用计算函数开始计算
//        cout<<"The Detection Result:"<<endl;
//        cout<<"HOG dims: "<<descriptors.size()<<endl;
//        Mat SVMtrainMat =  Mat::zeros(1,descriptors.size(),CV_32FC1);
//        n=0;
//        for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)
//        {
//            SVMtrainMat.at<float>(0,n) = *iter;
//            n++;
//        }
//        
//        
//        CvSVM svm ;
//        string loadpath=inputpath+"SVM_DATA.xml";
//        const char * load_str=loadpath.c_str();
//        svm.load(load_str);
// 
//        
//        
//        int ret = svm.predict(SVMtrainMat);
//        std::sprintf( line, "%s %d\r\n", img_tst_path[j].c_str(), ret );
//        printf("%s %d\r\n", img_tst_path[j].c_str(), ret);//输出预测的结果，ret的值就代表类别
//        //getchar();
//        predict_txt<<line;
//    }
//    predict_txt.close();
//    system("PAUSE");
//
//    return;
//    
//
//
//}
