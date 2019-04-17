//
//  smooth.hpp
//  ttt
//
//  Created by 朱瑞 on 17/7/29.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#ifndef smooth_hpp
#define smooth_hpp

#include <iostream>
#include<string>
using namespace std;
class SmoothDetection{
    
public:
    void imgProcess(std::string inputpath,std::string outpath);
    void showSmoothCommonInit(std::string init,std::string mark,string out);
    void showSmoothCommonLine(int minlimit,int maxlimit, string init,string markedpath,string outpath);

//
//    void showSmoothLine(int minlimit,int maxlimit,string init,string mark,string out);
//    void drawSmoothBlack(int minlimit,int maxlimit,string src,string gray,string out);
//    void drawSmoothRaw(int minlimit,int maxlimit,string src,string gray,string out);
    
    
};//class

#endif /* smooth_hpp */
