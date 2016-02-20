#ifndef JIMAGE_H
#define JIMAGE_H

#define PI 3.1415926

#include <QImage>
#include <math.h>
#include <stdio.h>

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
    float *DCT_Matrix;
    float *DCT_Matrix_Tran;
    int *store_Matrix;
    
    void Calculate_DCT_Matrix(float matrix[64]);
    void Calculate_DCT_Matrix_Tran(float matrix[64],float result[64]);
    void Matrix_Multiply(float matrix1[64], float matrix2[64], float result[64]);
    int Subsample(int a[4]);
    void LoopSubsample(QImage image);
    void DCT(QImage image, QImage &target);
    void Quantize_Y(QImage image, QImage &target);
    void Quantize_CBCR(QImage image, QImage &target);
    void DQuantize_Y(QImage image, QImage &target);
    void DQuantize_CBCR(QImage image, QImage &target);
    void DDCT(QImage image, QImage &target);
    void Decode();
    
};

#endif // JIMAGE_H
