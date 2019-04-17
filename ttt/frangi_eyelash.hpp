//
//  frangi_eyelash.hpp
//  ttt
//
//  Created by 朱瑞 on 17/8/6.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#ifndef frangi_eyelash_hpp
#define frangi_eyelash_hpp


#include <stdio.h>

#include <opencv2/opencv.hpp>


//options for the filter
typedef struct{
    //vessel scales
    int sigma_start;
    int sigma_end;
    int sigma_step;
    
    //BetaOne: suppression of blob-like structures.
    //BetaTwo: background suppression. (See Frangi1998...)
    float BetaOne;
    float BetaTwo;
    
    bool BlackWhite; //enhance black structures if true, otherwise enhance white structures
} frangi2d_opts_t_eyelash;




/////////////////
//Frangi filter//
/////////////////

//apply full Frangi filter to src. Vesselness is saved in J, scale is saved to scale, vessel angle is saved to directions.
void frangi2d_eyelash(const cv::Mat &src, cv::Mat &J, cv::Mat &scale, cv::Mat &directions, frangi2d_opts_t_eyelash opts);



////////////////////
//Helper functions//
////////////////////

//run 2D Hessian filter with parameter sigma on src, save to Dxx, Dxy and Dyy.
void frangi2d_hessian_eyelash(const cv::Mat &src, cv::Mat &Dxx, cv::Mat &Dxy, cv::Mat &Dyy, float sigma);

//set opts to default options (sigma_start = 3, sigma_end = 7, sigma_step = 1, BetaOne = 1.6, BetaTwo 0.08)
void frangi2d_createopts_eyelash(frangi2d_opts_t_eyelash *opts);

//estimate eigenvalues from Dxx, Dxy, Dyy. Save results to lambda1, lambda2, Ix, Iy.
void frangi2_eig2image_eyelash(const cv::Mat &Dxx, const cv::Mat &Dxy, const cv::Mat &Dyy, cv::Mat &lambda1, cv::Mat &lambda2, cv::Mat &Ix, cv::Mat &Iy);







#endif /* frangi_eyelash_hpp */
