#include "jimage.h"
#include <iostream>

using namespace std;
const int Luminance_Quantization_Table[64] =                //Non-uniform Quantization table for Y
{
    16,  11,  10,  16,  24,  40,  51,  61,
    12,  12,  14,  19,  26,  58,  60,  55,
    14,  13,  16,  24,  40,  57,  69,  56,
    14,  17,  22,  29,  51,  87,  80,  62,
    18,  22,  37,  56,  68, 109, 103,  77,
    24,  35,  55,  64,  81, 104, 113,  92,
    49,  64,  78,  87, 103, 121, 120, 101,
    72,  92,  95,  98, 112, 100, 103,  99
};

const int Luminance_Quantization_Constant_Table[64] =             //Constant Quantization table for Y
{
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32
};

const int Chrominance_Quantization_Table[64] =                    //Non-uniform Quantization table for Cr,Cb
{
    17,  18,  24,  47,  99,  99,  99,  99,
    18,  21,  26,  66,  99,  99,  99,  99,
    24,  26,  56,  99,  99,  99,  99,  99,
    47,  66,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99
};

const int Chrominance_Quantization_Constant_Table[64] =                 //Constant Quantization table for Cb, Cr
{
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32
};



JImage::JImage()
{
    this->quality = 100;
    this->constant = 0;
    this->flag =0;
    this->DCT_Matrix = new float[64];
    this->DCT_Matrix_Tran = new float[64];
    Calculate_DCT_Matrix(this->DCT_Matrix);
    Calculate_DCT_Matrix_Tran(this->DCT_Matrix,this->DCT_Matrix_Tran);
    this->store_Matrix_Y = NULL;
    this->store_Matrix_Cb = NULL;
    this->store_Matrix_Cr = NULL;
}

void JImage::Calculate_DCT_Matrix(float matrix[64])                                      //calculate the DCT matrix T
{
    for (int i = 0; i<8;i++)
    {
        for(int j = 0; j<8;j++)
        {
            matrix[8*i+j] = (i==0) ? 1/sqrt(8) : 0.5*cos((2*j+1)*i*PI/16);                //formula in PPT
        }
    }
}

void JImage::Calculate_DCT_Matrix_Tran(float matrix[64],float result[64])                 //matrix transform to calculate the Transpose T
{
    for (int i = 0; i<8;i++)
    {
        for(int j = 0; j<8;j++)
        {
            result[8*j+i] = matrix[8*i+j];                                              //T[j,i] = Transpose T[i,j]
        }
    }
}

void JImage::Matrix_Multiply(float matrix1[64], float matrix2[64], float result[64])            //matrix multiplication
{
    for (int i = 0; i<8;i++)
    {
        for(int j = 0; j<8;j++)
        {
            result[8*i+j] = 0;
            for(int k =0; k<8;k++)
            {
                result[8*i+j] += matrix1[i*8+k]* matrix2[k*8+j];
            }
        }
    }
}


void JImage::ChangeQuality(int quality)
{
    this->quality = quality;
}

void JImage::ChangeConst(bool constant)
{
    this->constant = constant;
}

void JImage::ChangeImage(QImage origin)                                    //initial the image if it changes
{
    this->flag = 0;
    this->origin = origin;
}

void JImage::UpdateImage()                                                //after apply submit to update all the step
{
    if(this->flag == 0)
    {
        LoopSubsample(this->origin);
        DCT(Y,DCT_Y,store_Matrix_Y);
        DCT(CB,DCT_U,store_Matrix_Cb);
        DCT(CR,DCT_V,store_Matrix_Cr);
    }

    Quantize_Y(DCT_Y,Quan_Y,store_Matrix_Y);
    Quantize_CBCR(DCT_U,Quan_U,store_Matrix_Cb);
    Quantize_CBCR(DCT_V,Quan_V,store_Matrix_Cr);

    DQuantize_Y(Quan_Y,DQY,store_Matrix_Y);
    DQuantize_CBCR(Quan_U,DQU,store_Matrix_Cb);
    DQuantize_CBCR(Quan_V,DQV,store_Matrix_Cr);

    DDCT(DCT_Y,DDCTY,store_Matrix_Y);
    DDCT(DQU,DDCTU,store_Matrix_Cb);
    DDCT(DQV,DDCTV,store_Matrix_Cr);

    Decode();

}

int JImage::Subsample(int a[4])                                          //transform four value of colour to one
{
    return int((a[0]+a[1]+a[2]+a[3])/4);
}

void JImage::LoopSubsample(QImage image)                                   //transform from RGB to YCbCr and subsample
{
    int o_width = image.width();
    int o_height = image.height();
    if(store_Matrix_Y != NULL ){
        delete[] store_Matrix_Y;
        delete[] store_Matrix_Cb;
        delete[] store_Matrix_Cr;
    }
    
    o_width  -= o_width % 16;                                                //discard the severa pixel to let the image can do 8*8 blocks function.
    o_height -= o_height % 16;
    store_Matrix_Y = new int[o_width*o_height+10];                           //int array to store the every step's data
    store_Matrix_Cb = new int[o_width*o_height/4+10];
    store_Matrix_Cr = new int[o_width*o_height/4+10];
    
    int R,G,B;
    int ydata,cbdata[4],crdata[4];

    this->Y = QImage( o_width, o_height, QImage::Format_RGB32);                       //use QImage to store the seperate value to show up
    this->CB = QImage( o_width/2, o_height/2, QImage::Format_RGB32);
    this->CR = QImage( o_width/2, o_height/2, QImage::Format_RGB32);

    QRgb temp;
    int average;

    for(int i = 0; i< o_width; i=i+2 )
    {
        for(int j = 0; j< o_height;j=j+2)
        {
            for(int x= 0; x<2;x++)
            {
                for(int y =0;y<2;y++){
                    R= qRed(image.pixel(i+x,j+y));
                    G = qGreen(image.pixel(i+x,j+y));
                    B = qBlue(image.pixel(i+x,j+y));

                    ydata = (int)(0.299f * R + 0.587f * G + 0.114f * B);                               //formulate to use to transform form RGB to YCbCr
                    cbdata[x+2*y] = (int)(-0.168736f * R - 0.331264f * G + 0.5f * B + 128);
                    crdata[x+2*y] = (int)(0.5f * R - 0.418688f * G - 0.081312f * B + 128);

                    temp = qRgb(ydata,ydata,ydata);
                    Y.setPixel(i+x,j+y,temp);                                                         //every pixel store Y value
                    store_Matrix_Y[i+x+(j+y)*o_width]=ydata;
                }
            }

            average = Subsample(cbdata);                                                            //every four pixel to store Cb.Cr value
            temp = qRgb(average,average,average);
            CB.setPixel(floor(i/2),floor(j/2),temp);
            int index = (int ) floor(i/2)+floor(j/2)*o_width/2;
            store_Matrix_Cb[index]=average;
            average = Subsample(crdata);
            temp = qRgb(average,average,average);
            CR.setPixel(floor(i/2),floor(j/2),temp);
            store_Matrix_Cr[index]=average;
        }
    }
}

void JImage::DCT(QImage image, QImage &target,int store_matrix[])                           //DCT function
{
    int o_width = image.width();
    int o_height = image.height();
    target = QImage( o_width, o_height, QImage::Format_RGB32);

    
    int value;
    QRgb colortemp;

    for(int i = 0; i< o_width; i=i+8 )
    {
        for(int j = 0; j< o_height;j=j+8)
        {
            float first[64];
            float f_matrix[64];
            float final[64];
            for(int x=0; x<8 ;x++)
            {
                for(int y=0;y<8;y++)
                {

                    f_matrix[8*y+x] = store_matrix[i+x+(j+y)*o_width];                  //   retrieve data from store_matrix
                }
            }
            Matrix_Multiply(this->DCT_Matrix,f_matrix,first);                            //use formula F = T*f*T Transpose
            Matrix_Multiply(first,this->DCT_Matrix_Tran,final);

            for(int x=0; x<8 ;x++)
            {
                for(int y=0;y<8;y++)
                {
                    value = (int) (final[y*8+x]);
                    colortemp = qRgb(value,value,value);
                    target.setPixel(i+x,j+y,colortemp);

                    store_matrix[i+x+(j+y)*o_width]=value;                           //store the new data into store matrix and show up Qimage
                }
            }
        }
    }
}


void JImage::Quantize_Y(QImage image, QImage &target,int store_matrix[])            //do Quantization function for Y
{
    int o_width = image.width();
    int o_height = image.height();
    int value;
    QRgb colortemp;
    target = QImage( o_width, o_height, QImage::Format_RGB32);

    for(int i = 0; i< o_width; i++ )
    {
        for(int j = 0; j< o_height;j++)
        {
            value = store_matrix[i+j*o_width];
            if(constant == false){                                                      //choose whether constant quantization or non-uniform quantization
                value = value/ round((Luminance_Quantization_Table[(j%8)*8+(i%8)]*quality+50.0)/100);          //do quatization use quality value
            }
            else
            {
                value = value/ round(( Luminance_Quantization_Constant_Table[(j%8)*8+(i%8)]*quality+50.0)/100);
            }

            colortemp = qRgb(value,value,value);
            target.setPixel(i,j,colortemp);
            store_matrix[i+j*o_width]=value;
        }
    }
}

void JImage::Quantize_CBCR(QImage image, QImage &target,int store_matrix[])                //do Quantization function for Cb, Cr
{
    int o_width = image.width();
    int o_height = image.height();
    int value;
    QRgb colortemp;
    target = QImage( o_width, o_height, QImage::Format_RGB32);

    for(int i = 0; i< o_width; i++ )
    {
        for(int j = 0; j< o_height;j++)
        {
            value = store_matrix[i+j*o_width];
            if(constant == false){
                value = value/ round((Chrominance_Quantization_Table[(j%8)*8+(i%8)]*quality+50.0)/100);
            }
            else
            {
                value = value/ round((Chrominance_Quantization_Constant_Table[(j%8)*8+(i%8)]*quality+50.0)/100);
            }
            colortemp = qRgb(value,value,value);
            target.setPixel(i,j,colortemp);
            store_matrix[i+j*o_width] = value;
        }
    }
}

void JImage::DQuantize_Y(QImage image, QImage &target,int store_matrix[])              //do De-Quantization function for Y
{
    int o_width = image.width();
    int o_height = image.height();
    int value;
    QRgb colortemp;
    target = QImage( o_width, o_height, QImage::Format_RGB32);

    for(int i = 0; i< o_width; i++ )
    {
        for(int j = 0; j< o_height;j++)
        {
            value = store_matrix[i+j*o_width];

            if(constant == false){
                value = value*round(( Luminance_Quantization_Table[(j%8)*8+(i%8)]*quality+50.0)/100);
            }
            else
            {
                value = value*round(( Luminance_Quantization_Constant_Table[(j%8)*8+(i%8)]*quality+50.0)/100);
            }

            colortemp = qRgb(value,value,value);
            target.setPixel(i,j,colortemp);
            store_matrix[i+j*o_width]= value;
        }
    }
}

void JImage::DQuantize_CBCR(QImage image, QImage &target,int store_matrix[])          //do De-Quantization function for Cb,Cr
{
    int o_width = image.width();
    int o_height = image.height();
    int value;
    QRgb colortemp;
    target = QImage( o_width, o_height, QImage::Format_RGB32);

    for(int i = 0; i< o_width; i++ )
    {
        for(int j = 0; j< o_height;j++)
        {
            value = store_matrix[i+j*o_width];

            if(constant == false){
                value = value*round(( Chrominance_Quantization_Table[(j%8)*8+(i%8)]*quality+50.0)/100);
            }
            else
            {
                value = value*round((Chrominance_Quantization_Constant_Table[(j%8)*8+(i%8)]*quality+50.0)/100);
            }
            colortemp = qRgb(value,value,value);
            target.setPixel(i,j,colortemp);
            store_matrix[i+j*o_width] = value;
        }
    }
}

void JImage::DDCT(QImage image, QImage &target,int store_matrix[])                      //do inverse DCT for the image
{
    int o_width = image.width();
    int o_height = image.height();
    target = QImage( o_width, o_height, QImage::Format_RGB32);

    int value;
    QRgb colortemp;

    for(int i = 0; i< o_width; i=i+8 )
    {
        for(int j = 0; j< o_height;j=j+8)
        {
            float first[64];
            float f_matrix[64];
            float final[64];
            for(int x=0; x<8 ;x++)
            {
                for(int y=0;y<8;y++)
                {
                    f_matrix[8*y+x] = (float) store_matrix[i+x+(j+y)*o_width];
                }
            }
            Matrix_Multiply(this->DCT_Matrix_Tran,f_matrix,first);                            //use formula to calculate the f = transpose T*F*T
            Matrix_Multiply(first,this->DCT_Matrix,final);

            for(int x=0; x<8 ;x++)
            {
                for(int y=0;y<8;y++)
                {
                    value = (int) (final[y*8+x]);
                    colortemp = qRgb(value,value,value);
                    target.setPixel(i+x,j+y,colortemp);
                    store_matrix[i+x+(y+j)*o_width] = value;
                }
            }
        }
    }

}

void JImage::Decode()                          //transform the YCbCr value to RGB
{
    int o_width = Y.width();
    int o_height = Y.height();
    int R,G,B;
    int ydata,cbdata,crdata;


    this->decoded = QImage( o_width, o_height, QImage::Format_RGB32);

    QRgb temp;

    for(int i = 0; i< o_width; i++ )
    {
        for(int j = 0; j< o_height;j++)
        {

            ydata= store_Matrix_Y[i+j*o_width];
            int index = floor(i/2)+floor(j/2)*o_width/2;
            cbdata = store_Matrix_Cb[index];
            crdata = store_Matrix_Cr[index];

            R = round( ydata                            + 1.402f * (crdata-128) );              //if the RGB value is out of 0-255 fix it
            G = round( ydata   - 0.34414f*(cbdata-128)  -0.71414f* (crdata-128) );
            B = round( ydata   + 1.772f * (cbdata-128)                          );
            R = (R<0)?0:R;
            R = (R>255)?255:R;
            G = (G<0)?0:G;
            G = (G>255)?255:G;
            B = (B<0)?0:B;
            B = (B>255)?255:B;

            temp = qRgb(R,G,B);
            decoded.setPixel(i,j,temp);

        }
    }
}






