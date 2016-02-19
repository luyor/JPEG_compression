#ifndef JIMAGE_H
#define JIMAGE_H

#include <QImage>

class JImage
{
public:
    JImage();
    void ChangeQuality(int quality);
    void ChangeConst();
    void ChangeImage(QImage origin);
    
    QImage origin;
    QImage Y,U,V;
    QImage DCT_Y,DCT_U,DCT_V;
    QImage Quan_Y,Quan_U,Quan_V;
    QImage DQY,DQU,DQV;
    QImage DDCTY,DDCTU,DDCTV;
    QImage decoded;
    
private:
    int quality;
    bool constant;
    
    int Subsample(int a[2][2]);
    void LoopSubsample(QImage image);
    void DCT(QImage image, QImage &target);
    void Quantize(QImage image, QImage &target);
    void DQuantize(QImage image, QImage &target);
    void DDCT(QImage image, QImage &target);
    void Decode(QImage image, QImage &target);
};

#endif // JIMAGE_H
