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
    

    QImage origin;                             //read the origin image
    QImage Y,CB,CR;                            //Y,Cb,Cr value use gray image to show and also other steps' values
    QImage DCT_Y,DCT_U,DCT_V;
    QImage Quan_Y,Quan_U,Quan_V;
    QImage DQY,DQU,DQV;
    QImage DDCTY,DDCTU,DDCTV;
    QImage decoded;
    
private:
    int quality;                                //store the choose quality to compress
    bool constant;                              //to determine which kind of quantizaiton is
    bool flag;                                  //whether this image is the first time to load
    float *DCT_Matrix;                          // T matrix
    float *DCT_Matrix_Tran;                     //Transpose T matrix
    int *store_Matrix_Y;                        // three array to store the value for transfer
    int *store_Matrix_Cb;
    int *store_Matrix_Cr;
    
    void Calculate_DCT_Matrix(float matrix[64]);
    void Calculate_DCT_Matrix_Tran(float matrix[64],float result[64]);
    void Matrix_Multiply(float matrix1[64], float matrix2[64], float result[64]);
    int Subsample(int a[4]);
    void LoopSubsample(QImage image);
    void DCT(QImage image, QImage &target,int store_matrix[]);
    void Quantize_Y(QImage image, QImage &target,int store_matrix[]);
    void Quantize_CBCR(QImage image, QImage &target,int store_matrix[]);
    void DQuantize_Y(QImage image, QImage &target,int store_matrix[]);
    void DQuantize_CBCR(QImage image, QImage &target,int store_matrix[]);
    void DDCT(QImage image, QImage &target,int store_matrix[]);
    void Decode();
    
};

#endif // JIMAGE_H
