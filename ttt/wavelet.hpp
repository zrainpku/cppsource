//
//  wavelet.hpp
//  ttt
//
//  Created by 朱瑞 on 17/8/1.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#ifndef wavelet_hpp
#define wavelet_hpp



#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Wavelet
{
public:
    Wavelet(int layer);
    ~Wavelet();
    Mat ColorImageEqualize(Mat src);         //
    Mat WaveletImage(Mat src);               //
    
private:
    int _layer;
    void DWT(IplImage *pImage, int nLayer);  // 
    void IDWT(IplImage *pImage, int nLayer); //
};

#endif /* wavelet_hpp */
