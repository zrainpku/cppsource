//
//  eyelash.hpp
//  ttt
//
//  Created by 朱瑞 on 17/7/31.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#ifndef eyelash_hpp
#define eyelash_hpp

#include<string>
#include <opencv2/opencv.hpp>
class KEyelashDetection{
    
    
private:
    cv::Mat matSrc;
    cv::Mat matTemp;
    cv::Mat matEyeLine;
    cv::Mat matAns;
    cv::Mat findEyeLine(cv::Mat src);
    cv::Mat imgProcess();
    cv::Mat showEyelashCommonInit();

    
public:
    KEyelashDetection(cv::Mat mat);
    cv::Mat KGetEyelash();
    ~KEyelashDetection();
    

    
};//class

#endif /* eyelash_hpp */
