#include "jimage.h"
#include <iostream>

using namespace std;
const int Luminance_Quantization_Table[64] =
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

const int Chrominance_Quantization_Table[64] =
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



JImage::JImage()
{
    this->quality = 100;
    this->constant = 0;
    this->flag =0;
    this->DCT_Matrix = new float[64];
    this->DCT_Matrix_Tran = new float[64];
    Calculate_DCT_Matrix(this->DCT_Matrix);
    Calculate_DCT_Matrix_Tran(this->DCT_Matrix,this->DCT_Matrix_Tran);
    //cout << DCT_Matrix[11] << " " << DCT_Matrix[21] << endl;
    //cout << DCT_Matrix_Tran[25] << " " << DCT_Matrix_Tran[42] << endl;
}

void JImage::Calculate_DCT_Matrix(float matrix[64])
{
    for (int i = 0; i<8;i++)
    {
        for(int j = 0; j<8;j++)
        {
            matrix[8*i+j] = (i==0) ? 1/sqrt(8) : 0.5*cos((2*j+1)*i*PI/16);
        }
    }
    cout << matrix[0] << " " << matrix[63] << endl;

}

void JImage::Calculate_DCT_Matrix_Tran(float matrix[64],float result[64])
{
    for (int i = 0; i<8;i++)
    {
        for(int j = 0; j<8;j++)
        {
            result[8*j+i] = matrix[8*i+j];
        }
    }
}

void JImage::Matrix_Multiply(float matrix1[64], float matrix2[64], float result[64])
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

void JImage::ChangeImage(QImage origin)
{
    this->flag = 0;
    this->origin = origin;
}

void JImage::UpdateImage()
{
    if(this->flag == 0)
    {
        LoopSubsample(this->origin);
        DCT(Y,DCT_Y);
        //DCT(CB,DCT_U);
        //DCT(CR,DCT_V);
    }
    /*
    Quantize_Y(DCT_Y,Quan_Y);
    Quantize_CBCR(DCT_U,Quan_U);
    Quantize_CBCR(DCT_V,Quan_V);

    DQuantize_Y(Quan_Y,DQY);
    DQuantize_CBCR(Quan_U,DQU);
    DQuantize_CBCR(Quan_V,DQV);
*/
    DDCT(DCT_Y,DDCTY);
    //DDCT(DQU,DDCTU);
    //DDCT(DQV,DDCTV);

    //Decode();

}

int JImage::Subsample(int a[4])
{
    return int((a[0]+a[1]+a[2]+a[3])/4);
}

void JImage::LoopSubsample(QImage image)
{
    int o_width = image.width();
    int o_height = image.height();
    if(store_Matrix_Y != NULL ){
        delete store_Matrix_Y;
        delete store_Matrix_Cb;
        delete store_Matrix_Cr;
        
    }
    
    o_width  -= o_width % 16;
    o_height -= o_height % 16;
    int *store_Matrix_Y = new int[o_width][o_height];
    int *store_Matrix_Cb = new int[o_width/2][o_height/2];
    int *store_Matrix_Cr = new int[o_width/2][o_height/2];
    
    int R,G,B;
    int ydata,cbdata[4],crdata[4];

    this->Y = QImage( o_width, o_height, QImage::Format_RGB32);
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

                    ydata = (int)(0.299f * R + 0.587f * G + 0.114f * B);
                    cbdata[x+2*y] = (int)(-0.168736f * R - 0.331264f * G + 0.5f * B + 128);
                    crdata[x+2*y] = (int)(0.5f * R - 0.418688f * G - 0.081312f * B + 128);

                    temp = qRgb(ydata,ydata,ydata);
                    Y.setPixel(i+x,j+y,temp);
                    store_Matrix_Y[]
                }
            }

            average = Subsample(cbdata);
            temp = qRgb(average,average,average);
            CB.setPixel(floor(i/2),floor(j/2),temp);
            average = Subsample(crdata);
            temp = qRgb(average,average,average);
            CR.setPixel(floor(i/2),floor(j/2),temp);
        }
    }
}

void JImage::DCT(QImage image, QImage &target)
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
                    f_matrix[8*y+x] = (float) qRed(image.pixel(i+x,j+y));
                }
            }
            Matrix_Multiply(this->DCT_Matrix,f_matrix,first);
            Matrix_Multiply(first,this->DCT_Matrix_Tran,final);
            
            //Matrix_Multiply(this->DCT_Matrix_Tran,final,first);
            //Matrix_Multiply(first,this->DCT_Matrix,final);

            for(int x=0; x<8 ;x++)
            {
                for(int y=0;y<8;y++)
                {
                    value = (int) (final[y*8+x]);
                    colortemp = qRgb(value,value,value);
                    target.setPixel(i+x,j+y,colortemp);
                }
            }
        }
    }
}


void JImage::Quantize_Y(QImage image, QImage &target)
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
            value = qRed(image.pixel(i,j));
            value = value/ Luminance_Quantization_Table[(j%8)*8+(i%8)];

            colortemp = qRgb(value,value,value);
            target.setPixel(i,j,colortemp);
        }
    }
}

void JImage::Quantize_CBCR(QImage image, QImage &target)
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
            value = qRed(image.pixel(i,j));
            value = value/ Chrominance_Quantization_Table[(j%8)*8+(i%8)];
            colortemp = qRgb(value,value,value);
            target.setPixel(i,j,colortemp);
        }
    }
}

void JImage::DQuantize_Y(QImage image, QImage &target)
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
            value = qRed(image.pixel(i,j));
            value = value* Luminance_Quantization_Table[(j%8)*8+(i%8)];
            colortemp = qRgb(value,value,value);
            target.setPixel(i,j,colortemp);
        }
    }
}

void JImage::DQuantize_CBCR(QImage image, QImage &target)
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
            value = qRed(image.pixel(i,j));
            value = value* Chrominance_Quantization_Table[(j%8)*8+(i%8)];
            colortemp = qRgb(value,value,value);
            target.setPixel(i,j,colortemp);
        }
    }
}

void JImage::DDCT(QImage image, QImage &target)
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
                    f_matrix[8*y+x] = (float) qRed(image.pixel(i+x,j+y));
                }
            }
            Matrix_Multiply(this->DCT_Matrix_Tran,f_matrix,first);
            Matrix_Multiply(first,this->DCT_Matrix,final);

            for(int x=0; x<8 ;x++)
            {
                for(int y=0;y<8;y++)
                {
                    value = (int) (final[y*8+x]);
                    colortemp = qRgb(value,value,value);
                    target.setPixel(i+x,j+y,colortemp);
                }
            }
        }
    }

}

void JImage::Decode()
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

            ydata= qRed(DDCTY.pixel(i,j));
            cbdata = qRed(DDCTU.pixel(i/2,j/2));
            crdata = qRed(DDCTV.pixel(i/2,j/2));

            R = (int)( ydata                            + 1.402f * (crdata-128) );
            G = (int)( ydata   - 0.34414f*(cbdata-128)  -0.71414f* (crdata-128) );
            B = (int)( ydata   + 1.772f * (cbdata-128)                          );

            temp = qRgb(R,G,B);
            decoded.setPixel(i,j,temp);

        }
    }
}






