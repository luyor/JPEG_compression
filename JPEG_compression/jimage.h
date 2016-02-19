#ifndef JIMAGE_H
#define JIMAGE_H

#define PI 3.1415926

#include <QImage>

class JImage
{
public:
    JImage();
    void ChangeQuality(int quality);
    void ChangeConst(bool constant);
    void ChangeImage(QImage origin);
    void UpdateImage();
    

    QImage origin;
    QImage Y,CB,CR;
    QImage DCT_Y,DCT_U,DCT_V;
    QImage Quan_Y,Quan_U,Quan_V;
    QImage DQY,DQU,DQV;
    QImage DDCTY,DDCTU,DDCTV;
    QImage decoded;
    
private:
    int quality;
    bool constant;
    bool flag;
    static const int* DCT_Matrix[64] = Calculate_DCT_Matrix();

    static int* Calculate_DCT_Matrix();
    int Subsample(int a[4]);
    void LoopSubsample(QImage image);
    void DCT(QImage image, QImage &target);
    void Quantize(QImage image, QImage &target);
    void DQuantize(QImage image, QImage &target);
    void DDCT(QImage image, QImage &target);
    void Decode();
};

#endif // JIMAGE_H
