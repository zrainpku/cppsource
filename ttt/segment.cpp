//
//  segment.cpp
//  ttt
//
//  Created by 朱瑞 on 17/8/29.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "segment.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <opencv2/opencv.hpp>
#ifndef byte
typedef unsigned char byte;
#endif

#ifndef _point_
#define _point_
struct point{int x;int y;};
#endif
#ifndef _rect_
#define _rect_
struct rect { int x0, y0, x1, y1; };
#endif

using namespace std;

void findSpotSeed(byte *imIO, int h, int w, int blockSize, int minDia, int midDia, int maxDia, int maxThred, int minThred)
{
    byte *imTemp = new byte[w*h];
    memcpy(imTemp, imIO, w*h);
    memset(imIO, 0, w*h);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            byte*imBlock = imTemp + i * w + j;
            
            double minMean = 0, midMean = 0, maxMean = 0, tempMean = 0;
            int minArea = 0, midArea = 0, maxArea = 0, tempArea = 0;
            
            for (int m = -1 * blockSize; m <= blockSize; m++) {
                for (int n = -1 * blockSize; n <= blockSize; n++) {
                    if ((abs(m) <= minDia / 2) && (abs(n) <= minDia / 2)) {
                        if ((i + m) >= 0 && (i + m) < h && (j + n) >= 0 && (j + n) < w) {
                            minMean += *(imBlock + m * w + n); minArea++;
                        }
                    }
                    else if ((abs(m) <= midDia / 2) && (abs(n) <= midDia / 2)) {
                        if ((i + m) >= 0 && (i + m) < h && (j + n) >= 0 && (j + n) < w) {
                            midMean += *(imBlock + m * w + n); midArea++;
                        }
                    }
                    else if ((abs(m) <= maxDia / 2) && (abs(n) <= maxDia / 2)) {
                        if ((i + m) >= 0 && (i + m) < h && (j + n) >= 0 && (j + n) < w) {
                            maxMean += *(imBlock + m * w + n); maxArea++;
                        }
                    }
                    else {
                        if ((i + m) >= 0 && (i + m) < h && (j + n) >= 0 && (j + n) < w) {
                            tempMean += *(imBlock + m * w + n); tempArea++;
                        }
                    }
                }
            }
            minMean /= minArea; midMean /= midArea; maxMean /= maxArea; tempMean /= tempArea;
            if (((minMean - midMean) > 0) && ((minMean - midMean) < maxThred)
                && ((midMean - maxMean) > 0) && ((midMean - maxMean) < maxThred) && ((minMean - tempMean) > minThred))
                *(imIO + i * w + j) = 255;
        }
    }
    delete[] imTemp;
}
void mergeSeed(byte* imIO, const int h, const int w, int dia, int num)
{
    int wh = h*w;
    byte* ptemp = new byte[wh];
    memcpy(ptemp, imIO, wh);
    memset(imIO, 0, wh);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int counter = 0;
            for (int m = dia / -2; m <= dia / 2; m++) {
                for (int n = dia / -2; n <= dia / 2; n++) {
                    if ((i + m) >= 0 && (i + m) < h &&
                        (j + n) >= 0 && (j + n) < w) {
                        if (ptemp[(i + m)*w + j + n] == 255)counter++;
                    }
                }
            }
            if (counter > num)imIO[i*w + j] = 255;
            else imIO[i*w + j] = 0;
        }
    }
    delete[]ptemp;
}
int bwLabel(const byte* im, const int h, const int w, int* imLabel)
{
    int wh = w*h;
    memset(imLabel, 0, wh * sizeof(int));
    short pDirction[16];
    pDirction[0] = 0; pDirction[1] = 1; pDirction[2] = 1; pDirction[3] = 1;
    pDirction[4] = 1; pDirction[5] = 0; pDirction[6] = 1; pDirction[7] = -1;
    pDirction[8] = 0; pDirction[9] = -1; pDirction[10] = -1; pDirction[11] = -1;
    pDirction[12] = -1; pDirction[13] = 0; pDirction[14] = -1; pDirction[15] = 1;
    
    unsigned long maxNum = wh;
    
    short* px = new short[maxNum];
    short* py = new short[maxNum];
    for (unsigned long i = 0; i<maxNum; ++i) {
        px[i] = -1, py[i] = -1;
    }
    
    int nLabelNo = 0;
    short* xBegin = px, *xEnd = px, *yBegin = py, *yEnd = py;
    for (int j = 0; j<w; ++j) {
        for (int i = h - 1; i >= 0; --i) {
            if (im[i * w + j] != 0) {
                if (imLabel[i*w + j] == 0) {
                    *xEnd = i, *yEnd = j;
                    ++xEnd, ++yEnd, nLabelNo += 1;
                    imLabel[i * w + j] = nLabelNo;
                    while (xBegin != xEnd) {
                        int ci = *xBegin, cj = *yBegin;
                        xBegin += 1, yBegin += 1;
                        for (int k = 0; k<8; ++k) {
                            int tempx = ci + pDirction[2 * k];
                            int tempy = cj + pDirction[2 * k + 1];
                            if (tempx<0 || tempx >= h || tempy<0 || tempy >= w)
                                continue;
                            if (im[tempx*w + tempy] == 0 ||
                                imLabel[tempx*w + tempy] != 0)
                                continue;
                            *xEnd = tempx, *yEnd = tempy;
                            ++xEnd; ++yEnd;
                            imLabel[tempx*w + tempy] = nLabelNo;
                        }
                    }
                    xBegin = px; xEnd = px; yBegin = py; yEnd = py;
                }
            }
        }
    }
    delete[] px;
    delete[] py;
    
    return nLabelNo;
}
void singleLabel(int *imLabel, byte *imROI, const int wh, const int grayLevel)
{
    memset(imROI, 0, wh);
    for (int i = 0; i < wh; i++) {
        if (imLabel[i] == grayLevel)
            imROI[i] = 255;
    }
}

bool findTopAndBottom(byte * im, const int h, const int w, int & top, int &bot)
{
    byte *pTop = im;
    byte *pBot = im + (w * h) - 1;
    bool isTop = false;
    bool isBot = false;
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (!isTop && ((*pTop) != 0)) {
                isTop = true, top = i;
            }
            if (!isBot && ((*pBot) != 0)) {
                isBot = true, bot = h - i - 1;
            }
            if (isTop && isBot) return true;
            pTop++, pBot--;
        }
    }
    return false;
}

bool findLeftAndRight(byte * im, const int h, const int w, int & l, int &r)
{
    bool isLeft = false;
    bool isRight = false;
    
    for (int j = 0; j < w; j++) {
        byte *pleft = im + j;
        byte *pright = im + w - 1 - j;
        for (int i = 0; i < h; i++) {
            if (!isLeft && ((*pleft) != 0)) {
                isLeft = true, l = j;
            }
            if (!isRight && ((*pright) != 0)) {
                isRight = true, r = w - j - 1;
            }
            if (isLeft && isRight)return true;
            pleft += w, pright += w;
        }
    }
    return false;
}

void amplifyEdge(const int h, const int w, rect & loc, int maxNum, double rate)
{
    int hSub = loc.y1 - loc.y0 + 1;
    int wSub = loc.x1 - loc.x0 + 1;
    int amplifyEdgeNum = min(maxNum, int(std::ceil(rate * hSub)));
    loc.y0 = max(loc.y0 - amplifyEdgeNum, 0);
    loc.y1 = min(loc.y1 + amplifyEdgeNum, h - 1);
    amplifyEdgeNum = min(maxNum, int(std::ceil(rate * wSub)));
    loc.x0 = max(loc.x0 - amplifyEdgeNum, 0);
    loc.x1 = min(loc.x1 + amplifyEdgeNum, w - 1);
}

int calGrayLevelNum(byte * im, int h, int w, int level)
{
    int counter = 0;
    for (int i = 0; i < w*h; i++) {
        if (im[i] == 255) counter++;
    }
    return counter;
}

void cropROI(byte* im, const int h, const int w, byte*imROI, const rect loc)
{
    int hSub = loc.y1 - loc.y0 + 1;
    int wSub = loc.x1 - loc.x0 + 1;
    
    for (int i = 0; i < hSub; i++) {
        byte* temp = im + (i + loc.y0)*w + loc.x0;
        for (int j = 0; j < wSub; j++) {
            imROI[i*wSub + j] = *temp++;
        }
    }
}
void  binary(byte* imIO, const int wh, const int thred)
{
    for (int i = 0; i < wh; i++) {
        if (imIO[i] > thred)imIO[i] = 255;
        else imIO[i] = 0;
    }
}

bool isBinaryAnd(byte*im1, byte* im2, const int h, const int w)
{
    for (int i = 0; i < w*h; i++) {
        if ((im1[i] == 0 || im1[i] == 255) &&
            (im2[i] == 0 || im2[i] == 255)) {
            if (im1[i] == 255 && im2[i] == 255) {
                return true;
            }
        }
    }
    return false;
}

bool binaryOrRect(byte * imIO, byte *roi, const int h, const int w, const rect loc)
{
    int hSub = loc.y1 - loc.y0 + 1;
    int wSub = loc.x1 - loc.x0 + 1;
    
    for (int i = loc.y0; i <= loc.y1; i++) {
        for (int j = loc.x0; j <= loc.x1; j++) {
            byte temp1 = imIO[i*w + j];
            byte temp2 = roi[(i - loc.y0)*wSub + j - loc.x0];
            if ((temp1 == 0 || temp1 == 255) &&
                (temp2 == 0 || temp2 == 255)) {
                imIO[i*w + j] = ((temp1 / 255) | (temp2 / 255)) * 255;
            }
            else return false;
        }
    }
    return true;
}

int otsuThred(const byte* im, const int wh)
{
    int value = 0;
    double diffVar, maxDiffVar = 0, foreRate = 0;
    double foreMean = 0, backMean = 0;
    int nNum = 0;
    byte *q = const_cast<byte*>  (im);
    for (long i = 0; i<wh; ++i) {
        if (*q > nNum)nNum = *q;
        ++q;
    }
    nNum += 1;
    long * pNum = new long[nNum];
    memset(pNum, 0, sizeof(long)*nNum);
    double pSum = 0;
    double sumForeGround = 0;
    double sumBackGround = 0;
    long ForePixNum = 0;
    long BackPixNum = 0;
    
    byte * p = const_cast<byte*>  (im);
    for (long j = 0; j<wh; j++) {
        pSum += ((*p)*1.0 / nNum);
        (*(pNum + (*p)))++;
        p++;
    }
    double pImageMean = pSum / wh;
    for (int i = 0; i<nNum; i++) {
        if (i == 0) {
            BackPixNum = 0;
            sumBackGround = 0;
        }
        else {
            BackPixNum += pNum[i - 1];
            sumBackGround += (i - 1)*1.0 / nNum*pNum[i - 1];
        }
        sumForeGround = pSum - sumBackGround;
        ForePixNum = wh - BackPixNum;
        foreRate = double(ForePixNum) / wh;
        if (ForePixNum == 0 || BackPixNum == 0)
            continue;
        foreMean = sumForeGround / ForePixNum;
        backMean = sumBackGround / BackPixNum;
        diffVar = foreRate*(foreMean - pImageMean)*(foreMean - pImageMean) + (1 - foreRate)*(backMean - pImageMean)*(backMean - pImageMean);
        if ((maxDiffVar < diffVar) || (abs(maxDiffVar - diffVar)<0.00000001)) {
            maxDiffVar = diffVar;
            value = i;
        }
    }
    delete[] pNum;
    return value;
}
void findTopAndBottomCrossRate(byte * im, const int h, const int w, double & topRate, double & botRate)
{
    byte *pTop = im;
    byte *pBot = im + (h - 1)*w;
    topRate = 0, botRate = 0;
    for (int i = 0; i < w; i++) {
        topRate += *pTop++;
        botRate += *pBot++;
    }
    if (w != 0) {
        topRate /= (255 * w);
        botRate /= (255 * w);
    }
}
void findLeftAndRightCrossRate(byte * im, const int h, const int w, double & lRate, double & rRate)
{
    byte *pL = im;
    byte *pR = im + (w - 1);
    lRate = 0, rRate = 0;
    for (int i = 0; i < h; i++) {
        lRate += (*pL);
        rRate += (*pR);
        pL += w, pR += w;
    }
    if (h != 0) {
        lRate /= (255 * h);
        rRate /= (255 * h);
    }
    
}
bool edgeAdjust(byte * im, int h, int w, double maxEdgeRate, double amplifyEdgeRate, rect& loc)
{
    string message;
    bool isAdjust = false;
    int hSub = loc.y1 - loc.y0 + 1;
    int wSub = loc.x1 - loc.x0 + 1;
    double edgeCross[4] = { 0,0,0,0 };//过边率
    findLeftAndRightCrossRate(im, hSub, wSub, edgeCross[0], edgeCross[1]);
    findTopAndBottomCrossRate(im, hSub, wSub, edgeCross[2], edgeCross[3]);
    int amplifyEdgeNum = min(40, int(std::ceil(amplifyEdgeRate * hSub)));
    if ((edgeCross[0]>maxEdgeRate) && (loc.x0 != 0)) {
        loc.x0 = max(loc.x0 - amplifyEdgeNum, 0);
        isAdjust = true;
    }
    if ((edgeCross[1]>maxEdgeRate) && (loc.x1 != w - 1)) {
        loc.x1 = min(loc.x1 + amplifyEdgeNum, w - 1);
        isAdjust = true;
    }
    amplifyEdgeNum = min(40, int(std::ceil(amplifyEdgeRate * wSub)));
    if ((edgeCross[2]>maxEdgeRate) && (loc.y0 != 0)) {
        loc.y0 = max(loc.y0 - amplifyEdgeNum, 0);
        isAdjust = true;
    }
    if ((edgeCross[3]>maxEdgeRate) && (loc.y1 != h - 1)) {
        loc.y1 = min(loc.y1 + amplifyEdgeNum, h - 1);
        isAdjust = true;
    }
    return isAdjust;
}
void binaryNo(byte* imIO, const int wh)
{
    for (int i = 0; i < wh; i++)
        imIO[i] = 255 - imIO[i];
}
void fillHole(const cv::Mat srcBw, cv::Mat &dstBw)
{
    cv::Size m_Size = srcBw.size();
    cv::Mat Temp = cv::Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像
    srcBw.copyTo(Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)));
    
    cv::floodFill(Temp, cv::Point(0, 0), cv::Scalar(255));
    
    cv::Mat cutImg;//裁剪延展的图像
    Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)).copyTo(cutImg);
    
    dstBw = srcBw | (~cutImg);
}
bool segmentionSpot(byte * imSeed, byte*im, byte* imSeg, int h, int w, double amplifyEdgeRate)
{
    vector<rect>rects;
    string message, outputFile;
    amplifyEdgeRate = 2;
    double maxEdgeRate = 0.2;
    int wh = w*h;
    std::memset(imSeg, 0, wh);
    int * imlabel = new int[wh];
    int labelNum = bwLabel(imSeed, h, w, imlabel);
    for (int iLabelNum = 1; iLabelNum <= labelNum; iLabelNum++) {
        byte * imSlgSeed = new byte[wh];
        singleLabel(imlabel, imSlgSeed, wh, iLabelNum);
        rect loc = { 0,0,0,0 };
        findTopAndBottom(imSlgSeed, h, w, loc.y0, loc.y1);
        findLeftAndRight(imSlgSeed, h, w, loc.x0, loc.x1);
        amplifyEdge(h, w, loc, 10, amplifyEdgeRate);
        bool isWhile = true;
        int counter = 0;
        //动态阈值
        int iterThred = 0;
        //动态过边率
        float iterCrossEdge = 0;
        while (isWhile) {
            isWhile = false;
            int hSub = loc.y1 - loc.y0 + 1;
            int wSub = loc.x1 - loc.x0 + 1;
            if (hSub >= h || wSub >= w)break;
            int hwSub = hSub*wSub;
            byte* seedROI = new byte[hwSub];
            byte* imROI = new byte[hwSub];
            cropROI(im, h, w, imROI, loc);
            cropROI(imSlgSeed, h, w, seedROI, loc);
            byte* binROI = new byte[hwSub];
            std::memcpy(binROI, imROI, hwSub);
            double levelROI = double(otsuThred(imROI, hwSub));
            //每次扩边阈值调低
            //levelROI += iterThred;
            binary(binROI, hwSub, levelROI);
            int* binROILabel = new int[hwSub];
            int roiLabelNum = bwLabel(binROI, hSub, wSub, binROILabel);
            for (int iroiLabelNum = 1; iroiLabelNum <= roiLabelNum; iroiLabelNum++) {
                byte * roiSlg = new byte[hwSub];
                singleLabel(binROILabel, roiSlg, hwSub, iroiLabelNum);
                if (isBinaryAnd(roiSlg, seedROI, hSub, wSub)) {
                    //只要进入必须退出循环
                    int areaRoiSlg = calGrayLevelNum(roiSlg, hSub, wSub, 255);
                    if (areaRoiSlg > 200) {
                        rect temprect = loc;
                        //每次过边率调低
                        //maxEdgeRate -= iterCrossEdge;
                        if (edgeAdjust(roiSlg, h, w, maxEdgeRate, amplifyEdgeRate, temprect)) {
                            counter++;
                            if (counter < 3) {
                                isWhile = true;
                                iterThred += 1;
                                iterCrossEdge += 0.1;
                                loc = temprect;
                                delete[]roiSlg;
                                break;
                            }
                        }
                        else {
                            isWhile = false;
                            binaryOrRect(imSeg, roiSlg, h, w, temprect);
                            rects.push_back(loc);
                        }
                    }
                    delete[]roiSlg;
                    break;
                }
                delete[]roiSlg;
            }
            
            delete[]binROILabel;
            delete[]binROI;
            delete[]seedROI;
            delete[]imROI;
        }
        delete[]imSlgSeed;
        
    }
    delete[] imlabel;
    cv::Mat imgGray(h, w, CV_8UC1, im);
    cv::Scalar tempVal = cv::mean(imgGray);
    float matMean = tempVal.val[0];
    int * imLabelFilter = new int[wh];
    byte* imTempSeg = new byte[wh];
    std::memcpy(imTempSeg, imSeg, wh);
    std::memset(imSeg, 0, wh);
    int labelFilterNum = bwLabel(imTempSeg, h, w, imLabelFilter);
    for (int iLabelNum = 1; iLabelNum <= labelFilterNum; iLabelNum++) {
        byte * imSlgSeed = new byte[wh];
        singleLabel(imLabelFilter, imSlgSeed, wh, iLabelNum);
        rect loc = { 0,0,0,0 };
        findTopAndBottom(imSlgSeed, h, w, loc.y0, loc.y1);
        findLeftAndRight(imSlgSeed, h, w, loc.x0, loc.x1);
        int hSub = loc.y1 - loc.y0 + 1;
        int wSub = loc.x1 - loc.x0 + 1;
        int hwSub = hSub*wSub;
        byte* seedROI = new byte[hwSub];
        byte* imROI = new byte[hwSub];
        cropROI(imSlgSeed, h, w, seedROI, loc);
        cropROI(im, h, w, imROI, loc);
        cv::Mat matROI(hSub, wSub, CV_8UC1, imROI);
        float roiMean = cv::mean(matROI).val[0];
        int areaSlgSeed = calGrayLevelNum(imSlgSeed, h, w, 255);
        cv::Mat fill(hSub, wSub, CV_8UC1, seedROI);
        fillHole(fill, fill);
        cv::Mat tempFill;
        fill.copyTo(tempFill);
        vector<vector<cv::Point> > contours;
        vector<cv::Vec4i> hierarchy;
        findContours(fill, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
        if (contours.size() == 1) {
            double s = contourArea(contours[0]);
            double l = arcLength(contours[0], true);
            double degree = 0.0;
            if (l != 0) degree = 4 * 3.1415926* s / pow(l, 2.0);
            if (degree > 0.3&& s>1000) {
                binaryOrRect(imSeg, tempFill.data, h, w, loc);
            }
            else if (s > 3000 && degree > 0.1) {
                binaryOrRect(imSeg, tempFill.data, h, w, loc);
            }
            else if (roiMean> 1.5 * matMean || roiMean>150) {
                binaryOrRect(imSeg, tempFill.data, h, w, loc);
            }
            
        }
        delete[] imROI;
        delete[] seedROI;
        delete[]imSlgSeed;
        
    }
    delete[]imLabelFilter;
    delete[]imTempSeg;
    
    return true;
}

void testSpot(string imgPath,string outPath)
{
    string message;
    clock_t start_time = clock();
    cv::Mat matColor = cv::imread(imgPath);
    int h = matColor.rows, w = matColor.cols, wh = w*h;
    //灰度
    cv::Mat matGray;
    cv::cvtColor(matColor, matGray, CV_BGR2GRAY);
    matGray = 255 - matGray;
    //cv::imwrite("D:/img/test/prepro/gray_no.jpg", matGray);
    //中值滤波
    cv::Mat matMedian;
    medianBlur(matGray, matMedian, 11);
    //cv::imwrite("D:/img/test/prepro/gray_no_median.jpg", matMedian);
    
    //预处理找种子点 开闭运算
    cv::Mat matClose, matOpen;
    cv::Mat eleOC = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(matMedian, matOpen, cv::MORPH_OPEN, eleOC);
    cv::morphologyEx(matOpen, matClose, cv::MORPH_CLOSE, eleOC);
    normalize(matClose, matClose, 0, 255, CV_MINMAX);
    //cv::imwrite("D:/img/test/prepro/gray_no_median_openclose.jpg", matClose);
    
    //预处理seg
    cv::Mat matTemp;
    //定位点获取
    byte* localIm = new byte[wh];
    memcpy(localIm, matClose.data, w*h);
    
    clock_t start_multi = clock();
    findSpotSeed(localIm, h, w, 10, 5, 7, 9, 150, 3);
    //mergeSeed(localIm, h, w, 9, 40);
    cv::Mat matLocal(h, w, CV_8UC1, localIm);
    cv::imwrite("D:/zhongzi.jpg", matLocal);
//    message.Format(_T("findSpotSeed cost is :%f"), (clock() - start_multi) / 1000.0);
    
//    LOGMESSAGE(_T(""), message);
    cv::Mat localMat(h, w, CV_8UC1, localIm);
    clock_t start_seg = clock();
    byte * imSeg = new byte[wh];
    segmentionSpot(localIm, matClose.data, imSeg, h, w, 2);
//    message.Format(_T("seg cost is :%f"), (clock() - start_seg) / 1000.0);
//    LOGMESSAGE(_T(""), message);
    //显示
    //cv::Mat dlmat;
    cv::Mat matShow(h, w, CV_8UC1, imSeg);
    cv::Mat eleShow = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(matShow, matTemp, cv::MORPH_DILATE, eleShow);
    matShow = matTemp - matShow;
    for (int i = 0; i<h; i++) {
        for (int j = 0; j<w; j++) {
            if (matShow.at<uchar>(i, j) == 255) {
                matColor.at<cv::Vec3b>(i, j)[0] = 255;
                matColor.at<cv::Vec3b>(i, j)[1] = 0;
                matColor.at<cv::Vec3b>(i, j)[2] = 0;
            }
            
        }
    }
//    string outPath;
//    outPath.Format("%s/整张图12.jpg", ExtractFileDir(imgPath));
//    cv::imwrite(outPath.GetBuffer(), matColor);
    cv::imwrite(outPath, matColor);
//    message.Format(_T("cost is :%f"), (clock() - start_time) / 1000.0);
//    LOGMESSAGE(_T(""), message);
    delete[]localIm;
    delete[]imSeg;
}
