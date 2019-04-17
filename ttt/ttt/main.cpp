//
//  main.cpp
//  ttt
//
//  Created by 朱瑞 on 17/7/10.
//  Copyright © 2017年 朱瑞. All rights reserved.


//#include "crop.hpp"
//#include "wrinkle.hpp"
//#include "pore.hpp"
//#include "smooth.hpp"
#include <opencv2/opencv.hpp>
//#include "eyelash.hpp"
//#include "blackHead.hpp"
//#include "hogSvm.hpp"
//#include "segment.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    string BasePath="/Users/zrain/Desktop/pic/";
    string BaseColorPath="/Users/zrain/Desktop/change/04DBX/";
    string WrinkleSource="/Users/zrain/Desktop/pic/wrinkle_source/";
    string WrinkleMid="/Users/zrain/Desktop/pic/wrinkle_mid/";
    string WrinkleResult="/Users/zrain/Desktop/pic/wrinkle_result/";
    
    string PoreSource="/Users/zrain/Desktop/pic/pore_source/";
    string PoreMid="/Users/zrain/Desktop/pic/pore_mid/";
    string PoreResult="/Users/zrain/Desktop/pic/pore_result/";

    string SmoothSource="/Users/zrain/Desktop/pic/Smooth_source/";
    string SmoothMid="/Users/zrain/Desktop/pic/Smooth_mid/";
    string SmoothResult="/Users/zrain/Desktop/pic/Smooth_result/";
    
    string EyelashSource="/Users/zrain/Desktop/pic/eyelash_source/";
    string EyelashMid="/Users/zrain/Desktop/pic/eyelash_mid/";
    string EyelashResult="/Users/zrain/Desktop/pic/eyelash_result/";
    string EyelashLabel="/Users/zrain/Desktop/pic/eyelash_label/";

    string LipSource="/Users/zrain/Desktop/pic/lip_source/";
    string LipMid="/Users/zrain/Desktop/pic/lip_mid/";
    string LipResult="/Users/zrain/Desktop/pic/lip_result/";
    string TrainData="/Users/zrain/Desktop/pic/train_data/";
    string TrainPath="/Users/zrain/Desktop/pic/train_path/";
    
    string Websource="/Users/zrain/Desktop/scshot/";
    string Webanswer="/Users/zrain/Desktop/scshot/";

    
    //**********web*******crop
    cv::Mat src_img=cv::imread(Websource+"test1.png");
//    KWrinkleDetect wd(src_img);
//    cv::Mat out_img=wd.getOutputImage();
    cv::imshow("test", src_img);
    cv::waitKey(0);
    
    
    //********web**end********
    
    
//**************wrinkle detection!
//            WrinkleDetect wd;

//        for(int i=9;i<=9;i++)
//        {
//            cout<<i<<endl;
//            char t[2];
//            sprintf(t, "%d",i);
////            cv::Mat src_img=cv::imread(WrinkleSource+"crop"+t[0]+".jpg");
//            cv::Mat src_img=cv::imread(WrinkleSource+"WechatIMG7" + ".jpg");
//
//            KWrinkleDetect KWD(src_img,100);
//            cv::Mat dest_img=KWD.getOutputImage();
////            cv::imwrite(WrinkleResult+"crop"+t[0]+"_markline.bmp", dest_img);
//            cv::imwrite(WrinkleResult+"WechatIMG7.jpg", dest_img);
//
//        }
    
    
//        string ans1=wd.imgToGray(WrinkleSource+"120.jpg",WrinkleMid+"wrinkle_gray.jpg");
//        string ans2=wd.grayToGrident(WrinkleMid+"wrinkle_gray.jpg",WrinkleMid+"wrinkle_gident.jpg");
//        string ans3=wd.gridenToFrangi(WrinkleMid+"wrinkle_gident.jpg", WrinkleMid+"wrinkle_frangi.bmp");
//        string ans4=wd.frangiToRidge(WrinkleMid+"wrinkle_frangi.bmp",WrinkleMid+"wrinkle_ridge.bmp");
//
//         wd.showWrinkleInit(WrinkleSource+"crop9.jpg",WrinkleMid+"wrinkle_ridge.bmp",WrinkleResult+"crop9_init.jpg");
//         wd.showWrinkleLine(400,1099,WrinkleSource+"crop9.jpg",WrinkleMid+"wrinkle_ridge.bmp",WrinkleResult+"crop9.bmp");
//         wd.drawWrinkleBlack(355,5000,WrinkleSource+"crop3.jpg",WrinkleMid+"wrinkle_ridge.jpg",WrinkleResult+"wrinkle3_drowarea.jpg");
//         wd.drawWrinkleRaw(355,5000,WrinkleSource+"crop3.jpg",WrinkleMid+"wrinkle_ridge.jpg",WrinkleResult+"wrinkle3_drowraw.jpg");

//*****************wrinkle
 
    // pore******
//    PoreDetection pd;
//    pd.imgPreprocess(PoreSource+"frontstand2.jpg", PoreMid+"pore_gray.jpg");
//    pd.imgToPore(PoreMid+"pore_gray.jpg", PoreMid+"pore_ridge.jpg");

//    pd.showPoreInit(PoreSource+"crop3.jpg",PoreMid+"pore_ridge.jpg",PoreResult+"crop3_markinit.jpg");
//    pd.showPoreLine(20,500,PoreSource+"frontstand2.jpg",PoreMid+"pore_ridge.jpg",PoreResult+"frontstand2_markline.jpg");
//    pd.drawPoreBlack(0,800,PoreSource+"stand22.jpg",PoreMid+"pore_ridge.jpg",PoreResult+"stand22_drowarea.jpg");
//    pd.drawPoreRaw(0,800,PoreSource+"stand22.jpg",PoreMid+"pore_ridge.jpg",PoreResult+"stand22_drowraw.jpg");

    
 
    //pore ************ end

    
    //smooth*************
//    SmoothDetection SD;
//    SD.imgProcess(SmoothSource+"crop9.jpg", SmoothMid);
//    SD.showSmoothCommonInit(SmoothSource+"stand11.jpg",SmoothMid,SmoothResult+"show_init.jpg");
//    SD.showSmoothCommonLine(255, 999999, SmoothSource+"crop9.jpg", SmoothMid, SmoothResult+"crop9.jpg");

    
    //smooth************
    
    
    
    //balckHead
//        PoreDetection pd;
//        pd.imgPreprocess(PoreSource+"blackhead1.jpg", PoreMid+"pore_gray.jpg");
//        pd.imgToPore(PoreMid+"pore_gray.jpg", PoreMid+"black_ridge.jpg");
//    findBlackHead(PoreSource+"blackhead1.jpg",PoreMid+"black_ridge.jpg",PoreResult+"blackhead3.jpg");
    //end**balckHead
    
    
    
    
    
    //*****eyelash*********
 /*
    for(int i=1;i<=1;i++)
    {
        char a[2],b[2],t[2];
        sprintf(a, "%d",0);
        sprintf(b, "%d",1);
        sprintf(t, "%d",i);
        
//        cv::Mat srcimg=cv::imread(EyelashSource+"eye"+t[0]+a[0]+".bmp");
//        KEyelashDetection ED0(srcimg);
//        cv::Mat matans=ED0.KGetEyelash();
//        cv::imwrite(EyelashResult+"eye"+t[0]+a[0]+".bmp",matans);
//        
//        srcimg=cv::imread(EyelashSource+"eye"+t[0]+b[0]+".bmp");
//        KEyelashDetection ED1(srcimg);
//        matans=ED1.KGetEyelash();
//        cv::imwrite(EyelashResult+"eye"+t[0]+b[0]+".bmp",matans);
        
        cv::Mat srcimg=cv::imread(EyelashSource+"WechatIMG74"+".bmp");
                KEyelashDetection ED1(srcimg);
        cv::Mat matans=ED1.KGetEyelash();
                cv::imwrite(EyelashResult+"WechatIMG74"+".bmp",matans);
  
    }

   */
    
    //*********eyelash********
    
    //*****eyelashlabel*********
    
//    EyelashDetection ED;
//    for(int i=1;i<=1;i++)
//    {
//        char a[2],b[2],t[2];
//        sprintf(a, "%d",0);
//        sprintf(b, "%d",1);
//        sprintf(t, "%d",i);
//        
//        ED.imgProcess2(EyelashSource+"eye"+t[0]+a[0]+".bmp",EyelashMid);
//        ED.createEyelashLabel(EyelashSource+"eye"+t[0]+a[0]+".bmp",EyelashMid,EyelashLabel+"eye"+t[0]+a[0]+".jpg");
//        ED.imgProcess2(EyelashSource+"eye"+t[0]+b[0]+".bmp",EyelashMid);
//        ED.createEyelashLabel(EyelashSource+"eye"+t[0]+a[0]+".bmp",EyelashMid,EyelashLabel+"eye"+t[0]+b[0]+".jpg");
//    }

    //*********eyelashlabel********
    
//    cv::GaussianBlur(matSrc, matSrc, cv::Size(111,111), 51,3);

    
    
//  ******************crop area
//    vector<cv::Point> points=getImagePoint(LipSource+"front1.jpg");
//    cv::Mat img1=cv::imread(LipSource+"front1.jpg");
//    drawLips(img1,points);
//    drawR33Eye(img1, points);
//    cv::imwrite(LipSource+"front11.jpg", img1);
    
//    cropImageROI(BasePath+"temp.jpg",BasePath+"temp1.jpg");
//    cropImageROI(PoreSource+"black1.jpg",PoreSource+"blackhead1.jpg");
//    cropImageROI(LipSource+"front1.jpg",LipSource+"lip1.jpg");
//    cropImageROI(LipSource+"front2.jpg",LipSource+"lip2.jpg");
//    cropImageROI(LipSource+"front3.jpg",LipSource+"lip3.jpg");
//    cropImageROI(LipSource+"front4.jpg",LipSource+"lip4.jpg");
//    cropImageROI(LipSource+"front5.jpg",LipSource+"lip5.jpg");

    
//    cropAcne(TrainData+"022/right.jpg",TrainData+"022/右面观丘疹.png",TrainData+"qiuzhen/");
//     trainSvmModel( TrainData);
//     testSvmModel(TrainData);
    
//    testSpot(TrainPath+"front2.jpg",TrainPath+"outfront2.jpg");


    
      return 0;
}
