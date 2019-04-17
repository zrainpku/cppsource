//
//  wrinkle_detection.hpp
//  ttt
//
//  Created by 朱瑞 on 17/7/10.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#ifndef wrinkle_detection_hpp
#define wrinkle_detection_hpp

#include<iostream>
#include<string>
//using namespace std;

class WrinkleDetection{
    
    
    
public:

    std::string imgToGray(std::string read_path,std::string write_path);
    std::string grayToGrident(std::string read_path,std::string write_path);
    std::string  gridenToFrangi(std::string read_path,std::string write_path) ;
    std::string frangiToRidge(std::string read_path,std::string write_path);
    std::string ridgeToThreshold(std::string read_path,std::string write_path);
    std::string ridgeToAdaptiveThreshold(std::string read_path,std::string write_path, int blocksize,int param1);
    void showWrinkleInit(std::string init,std::string mark,std::string out);
    void showWrinkleLine(int minlimit,int maxlimit,std::string init,std::string mark,std::string out);
    void drawWrinkleBlack(int minlimit, int maxlimit,std::string src,std::string gray,std::string out);
    void drawWrinkleRaw(int minlimit,int maxlimit,std::string src,std::string gray,std::string out);
    
    void imgToRidge(std::string inimage,std::string write_path);
    void showWrinkleCommonInit(std::string initImage,std::string markpath,std::string outpath);
    void showWrinkleCommonLine(int minlimit,int maxlimit,std::string init,std::string markedpath,std::string outpath);

    
};//class



#endif /* wrinkle_detection_hpp */
