#include "jimage.h"

const unsigned char Luminance_Quantization_Table[64] =
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

const unsigned char Chrominance_Quantization_Table[64] =
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

const char ZigZag[64] =
{
    0, 1, 5, 6,14,15,27,28,
    2, 4, 7,13,16,26,29,42,
    3, 8,12,17,25,30,41,43,
    9,11,18,24,31,40,44,53,
    10,19,23,32,39,45,52,54,
    20,22,33,38,46,51,55,60,
    21,34,37,47,50,56,59,61,
    35,36,48,49,57,58,62,63
};



JImage::JImage()
{
    this->quality = 100;
    this->constant = 0;
    this->flag =0;
}

void JImage::ChangeConst(int quality)
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
        DCT(U,DCT_U);
        DCT(V,DCT_V);
    }

    Quantize(DCT_Y,Quan_Y);
    Quantize(DCT_U,Quan_U);
    Quantize(DCT_V,Quan_V);

    DQuantize(Quan_Y,DQY);
    DQuantize(Quan_U,DQU);
    DQuantize(Quan_V,DQV);

    DDCT(DQY,DDCTY);
    DDCT(DQU,DDCTU);
    DDCT(DQV,DDCTV);

    Decode();

}

int JImage::Subsample(int a[4])
{
    return int((a[0]+a[1]+a[2]+a[3])/4);
}

int JImage::LoopSubsample(QImage image)
{
    int o_width = image.width();
    int o_height = image.height();
    int R,G,B;
    int ydata,cbdata[4],crdata[4];

    this->Y = QImage( o_width, o_height, 3);
    this->CB = QImage( ceil(o_width/2), ceil(o_height/2), 3);
    this->CR = QImage( ceil(o_width/2), ceil(o_height/2), 3);

    QRgb temp;
    int average;

    for(int i = 0; i< o_width; i=i+2 )
    {
        for(int j = 0; j< o_height/2;j=j+2)
        {
            for(int x= 0; x<2;x++)
            {
                for(int y =0;y<2;y++){
                    R= qRed(image.pixel(i+x,j+y));
                    G = qRed(image.pixel(i+x,j+y));
                    B = qRed(image.pixel(i+x,j+y));

                    ydata = (int)(0.299f * R + 0.587f * G + 0.114f * B);
                    cbdata[x+2*y] = (int)(-0.1687f * R - 0.3313f * G + 0.5f * B + 128);
                    crdata[x+2*y] = (int)(0.5f * R - 0.4187f * G - 0.0813f * B + 128);

                    temp = qRgb(ydata,ydata,ydata);
                    Y.setPixel(i+x,j+y,temp);
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
    QRgb colortemp;

    for(int i = 0; i< o_width; i=i+8 )
    {
        for(int j = 0; j< o_height/8;j=j+8)
        {
            for(int v=0; v<8; v++)
                {
                    for(int u=0; u<8; u++)
                    {
                        float alpha_u = (u==0) ? 1/sqrt(8.0f) : 0.5f;
                        float alpha_v = (v==0) ? 1/sqrt(8.0f) : 0.5f;

                        float temp = 0.f;
                        for(int x=0; x<8; x++)
                        {
                            for(int y=0; y<8; y++)
                            {
                                float data = qRed(image.pixel(i+x,j+y));

                                data *= cos((2*x+1)*u*PI/16.0f);
                                data *= cos((2*y+1)*v*PI/16.0f);

                                temp += data;
                            }
                        }

                        temp *= alpha_u*alpha_v;
                        colortemp = qRgb(int(temp),int(temp),int(temp));
                        target.setPixel(i+v,j+u,colortemp);
                    }
                }

        }
    }
}


void JImage::Quantize(QImage image, QImage &target)
{

}
void JImage::DQuantize(QImage image, QImage &target);
void JImage::DDCT(QImage image, QImage &target);
void JImage::Decode();




