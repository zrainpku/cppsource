//
//  wavelet.cpp
//  ttt
//
//  Created by 朱瑞 on 17/8/1.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include "wavelet.hpp"



Wavelet::Wavelet(int layer)
{
    _layer = layer;
}

Wavelet::~Wavelet()
{}

Mat Wavelet::ColorImageEqualize(Mat src)
{
    Mat equal_color;
    vector<Mat> splitBGR(src.channels()); // ²ÊÉ«Í¼ÏñÖ±·½Í¼¾ùºâ
    split(src, splitBGR);
    for (int i = 0; i < src.channels(); i++)
    {
        equalizeHist(splitBGR[i], splitBGR[i]);
    }
    merge(splitBGR, equal_color);
    return equal_color;
}

Mat Wavelet::WaveletImage(Mat src)
{
    Mat equal_src;
    vector<Mat> splitBGR(src.channels()); // ²ÊÉ«Í¼ÏñÖ±·½Í¼¾ùºâ
    split(src, splitBGR);
    for (int i = 0; i < src.channels(); i++)
    {
        equalizeHist(splitBGR[i], splitBGR[i]);
    }
    merge(splitBGR, equal_src);
    
    int nLayer = _layer;
    IplImage pSrc = equal_src;
    CvSize size = cvGetSize(&pSrc); // ¼ÆËãÐ¡²¨Í¼Ïó´óÐ¡
    IplImage *pWavelet = cvCreateImage(size, IPL_DEPTH_32F, pSrc.nChannels); // ´´½¨Ð¡²¨Í¼Ïó
    if (pWavelet)
    {
        // Ð¡²¨Í¼Ïó¸³Öµ
        cvConvertScale(&pSrc, pWavelet, 1, -128);
        // ²ÊÉ«Í¼ÏñÐ¡²¨±ä»»
        IplImage *pImage = cvCreateImage(cvGetSize(pWavelet), IPL_DEPTH_32F, 1);
        if (pImage)
        {
            for (int i = 1; i <= pWavelet->nChannels; i++)
            {
                cvSetImageCOI(pWavelet, i);
                cvCopy(pWavelet, pImage, NULL);
                
                // ¶þÎ¬ÀëÉ¢Ð¡²¨±ä»»
                DWT(pImage, nLayer);
                
                // ¶þÎ¬ÀëÉ¢Ð¡²¨»Ö¸´
                // IDWT(pImage, nLayer);
                cvCopy(pImage, pWavelet, NULL);
            }
            cvSetImageCOI(pWavelet, 0);
            cvReleaseImage(&pImage);
        }
        // Ð¡²¨±ä»»Í¼Ïó
        cvConvertScale(pWavelet, &pSrc, 1, 128);
        cvReleaseImage(&pWavelet);
    }
    Mat des(&pSrc, true);
    return des;
}


// ¶þÎ¬ÀëÉ¢Ð¡²¨±ä»»£¨µ¥Í¨µÀ¸¡µãÍ¼Ïñ£©
void Wavelet::DWT(IplImage *pImage, int nLayer)
{
    // Ö´ÐÐÌõ¼þ
    if (pImage)
    {
        if (pImage->nChannels == 1 &&
            pImage->depth == IPL_DEPTH_32F &&
            ((pImage->width >> nLayer) << nLayer) == pImage->width &&
            ((pImage->height >> nLayer) << nLayer) == pImage->height)
        {
            int i, x, y, n;
            float fValue = 0;
            float fRadius = sqrt(2.0f);
            int nWidth = pImage->width;
            int nHeight = pImage->height;
            int nHalfW = nWidth/2;
            int nHalfH = nHeight/2;
            float **pData = new float*[pImage->height];
            float *pRow = new float[pImage->width];
            float *pColumn = new float[pImage->height];
            for (i = 0; i < pImage->height; i++)
            {
                pData[i] = (float*) (pImage->imageData + pImage->widthStep * i);
            }
            // ¶à²ãÐ¡²¨±ä»»
            for (n = 0; n < nLayer; n++, nWidth /= 2, nHeight /= 2, nHalfW /= 2, nHalfH /= 2)
            {
                // Ë®Æ½±ä»»
                for (y = 0; y < nHeight; y++)
                {
                    // ÆæÅ¼·ÖÀë
                    memcpy(pRow, pData[y], sizeof(float) * nWidth);
                    for (i = 0; i < nHalfW; i++)
                    {
                        x = i * 2; pData[y][i] = pRow[x];
                        pData[y][nHalfW + i] = pRow[x + 1];
                    }
                    // ÌáÉýÐ¡²¨±ä»»
                    for (i = 0; i < nHalfW - 1; i++)
                    {
                        fValue = (pData[y][i] + pData[y][i + 1]) / 2;
                        pData[y][nHalfW + i] -= fValue;
                    }
                    fValue = (pData[y][nHalfW - 1] + pData[y][nHalfW - 2]) / 2;
                    pData[y][nWidth - 1] -= fValue;
                    fValue = (pData[y][nHalfW] + pData[y][nHalfW + 1]) / 4;
                    pData[y][0] += fValue;
                    for (i = 1; i < nHalfW; i++)
                    {
                        fValue = (pData[y][nHalfW + i] + pData[y][nHalfW + i - 1]) / 4;
                        pData[y][i] += fValue;
                    }
                    // Æµ´øÏµÊý
                    for (i = 0; i < nHalfW; i++)
                    {
                        pData[y][i] *= fRadius; pData[y][nHalfW + i] /= fRadius;
                    }
                }
                // ´¹Ö±±ä»»
                for (x = 0; x < nWidth; x++)
                {
                    // ÆæÅ¼·ÖÀë
                    for (i = 0; i < nHalfH; i++)
                    {
                        y = i * 2;
                        pColumn[i] = pData[y][x];
                        pColumn[nHalfH + i] = pData[y + 1][x];
                    }
                    for (i = 0; i < nHeight; i++)
                    {
                        pData[i][x] = pColumn[i];
                    }
                    // ÌáÉýÐ¡²¨±ä»»
                    for (i = 0; i < nHalfH - 1; i++)
                    {
                        fValue = (pData[i][x] + pData[i + 1][x]) / 2;
                        pData[nHalfH + i][x] -= fValue;
                    }
                    fValue = (pData[nHalfH - 1][x] + pData[nHalfH - 2][x]) / 2;
                    pData[nHeight - 1][x] -= fValue;
                    fValue = (pData[nHalfH][x] + pData[nHalfH + 1][x]) / 4;
                    pData[0][x] += fValue;
                    for (i = 1; i < nHalfH; i++)
                    {
                        fValue = (pData[nHalfH + i][x] + pData[nHalfH + i - 1][x]) / 4;
                        pData[i][x] += fValue;
                    }
                    // Æµ´øÏµÊý
                    for (i = 0; i < nHalfH; i++)
                    {
                        pData[i][x] *= fRadius;
                        pData[nHalfH + i][x] /= fRadius;
                    }
                }
            }
            delete[] pData;
            delete[] pRow;
            delete[] pColumn;
        }
    }
}


// ¶þÎ¬ÀëÉ¢Ð¡²¨»Ö¸´£¨µ¥Í¨µÀ¸¡µãÍ¼Ïñ£©
void Wavelet::IDWT(IplImage *pImage, int nLayer)
{
    // Ö´ÐÐÌõ¼þ
    if (pImage)
    {
        if (pImage->nChannels == 1 &&
            pImage->depth == IPL_DEPTH_32F &&
            ((pImage->width >> nLayer) << nLayer) == pImage->width &&
            ((pImage->height >> nLayer) << nLayer) == pImage->height)
        {
            int i, x, y, n;
            float fValue = 0;
            float fRadius = sqrt(2.0f);
            int nWidth = pImage->width >> (nLayer - 1);
            int nHeight = pImage->height >> (nLayer - 1);
            int nHalfW = nWidth/2;
            int nHalfH = nHeight/2;
            float **pData = new float*[pImage->height];
            float *pRow = new float[pImage->width];
            float *pColumn  = new float[pImage->height];
            for (i = 0; i < pImage->height; i++)
            {
                pData[i] = (float*) (pImage->imageData + pImage->widthStep * i);
            }
            // ¶à²ãÐ¡²¨»Ö¸´
            for (n = 0; n < nLayer; n++, nWidth *= 2, nHeight *= 2, nHalfW *= 2, nHalfH *= 2)
            {
                // ´¹Ö±»Ö¸´
                for (x = 0; x < nWidth; x++)
                {
                    // Æµ´øÏµÊý
                    for (i = 0; i < nHalfH; i++)
                    {
                        pData[i][x] /= fRadius;
                        pData[nHalfH + i][x] *= fRadius;
                    }
                    // ÌáÉýÐ¡²¨»Ö¸´
                    fValue = (pData[nHalfH][x] + pData[nHalfH + 1][x]) / 4;
                    pData[0][x] -= fValue;
                    for (i = 1; i < nHalfH; i++)
                    {
                        fValue = (pData[nHalfH + i][x] + pData[nHalfH + i - 1][x]) / 4;
                        pData[i][x] -= fValue;
                    }
                    for (i = 0; i < nHalfH - 1; i++)
                    {
                        fValue = (pData[i][x] + pData[i + 1][x]) / 2;
                        pData[nHalfH + i][x] += fValue;
                    }
                    fValue = (pData[nHalfH - 1][x] + pData[nHalfH - 2][x]) / 2;
                    pData[nHeight - 1][x] += fValue;
                    // ÆæÅ¼ºÏ²¢
                    for (i = 0; i < nHalfH; i++)
                    {
                        y = i * 2;
                        pColumn[y] = pData[i][x];
                        pColumn[y + 1] = pData[nHalfH + i][x];
                    }
                    for (i = 0; i < nHeight; i++)
                    {
                        pData[i][x] = pColumn[i];
                    }
                }
                // Ë®Æ½»Ö¸´
                for (y = 0; y < nHeight; y++)
                {
                    // Æµ´øÏµÊý
                    for (i = 0; i < nHalfW; i++)
                    {
                        pData[y][i] /= fRadius;
                        pData[y][nHalfW + i] *= fRadius;
                    }
                    // ÌáÉýÐ¡²¨»Ö¸´
                    fValue = (pData[y][nHalfW] + pData[y][nHalfW + 1]) / 4;
                    pData[y][0] -= fValue;
                    for (i = 1; i < nHalfW; i++)
                    {
                        fValue = (pData[y][nHalfW + i] + pData[y][nHalfW + i - 1]) / 4;
                        pData[y][i] -= fValue;
                    }
                    for (i = 0; i < nHalfW - 1; i++)
                    {
                        fValue = (pData[y][i] + pData[y][i + 1]) / 2;
                        pData[y][nHalfW + i] += fValue;
                    }
                    fValue = (pData[y][nHalfW - 1] + pData[y][nHalfW - 2]) / 2;
                    pData[y][nWidth - 1] += fValue;
                    // ÆæÅ¼ºÏ²¢
                    for (i = 0; i < nHalfW; i++)
                    {
                        x = i * 2;
                        pRow[x] = pData[y][i];
                        pRow[x + 1] = pData[y][nHalfW + i];
                    }
                    memcpy(pData[y], pRow, sizeof(float) * nWidth);
                }
            }
            delete[] pData;
            delete[] pRow;
            delete[] pColumn;
        }
    }
}
