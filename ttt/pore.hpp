//
//  pore.hpp
//  ttt
//
//  Created by 朱瑞 on 17/7/22.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#ifndef pore_hpp
#define pore_hpp

#include <iostream>
#include<string>
using namespace std;
class PoreDetection{
    
public:
    std::string imgPreprocess(std::string inputpath,std::string outpath);
    std::string imgToPore(std::string inputpath,std::string outpath);
    void showPoreInit(std::string init,std::string mark,string out);
    
    void showPoreLine(int minlimit,int maxlimit,string init,string mark,string out);
    void drawPoreBlack(int minlimit,int maxlimit,string src,string gray,string out);
    void drawPoreRaw(int minlimit,int maxlimit,string src,string gray,string out);


};//class

#endif /* pore_hpp */
