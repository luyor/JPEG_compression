#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_btn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
      tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
    QImage image;
    if (fileName.isEmpty()||!image.load(fileName)) {
        QDialog::tr("cannot read image file");
        return;
    }
    jimage.ChangeImage(image);
    
    QLabel *ql=findChild<QLabel *>("origin");
    ql->setPixmap(QPixmap::fromImage(jimage.origin));
    
    QLabel *ql2=findChild<QLabel *>("decoded");
    ql2->clear();
    QLabel *ql3=findChild<QLabel *>("Y");
    ql3->clear();
    QLabel *ql4=findChild<QLabel *>("U");
    ql4->clear();
    QLabel *ql5=findChild<QLabel *>("V");
    ql5->clear();
}

void MainWindow::on_nonuniform_clicked()
{
    jimage.ChangeConst(false);
}

void MainWindow::on_submit_btn_clicked()
{
    jimage.UpdateImage();
    QLabel *ql=findChild<QLabel *>("decoded");
    ql->setPixmap(QPixmap::fromImage(jimage.decoded));
    UpdateYUV();
}

void MainWindow::on_constant_clicked()
{
    jimage.ChangeConst(true);
}

void MainWindow::on_quality_editingFinished()
{
    QSpinBox *qsb=findChild<QSpinBox *>("quality");
    jimage.ChangeQuality(qsb->value());
}

void MainWindow::UpdateYUV(){
    QLabel *ql1=findChild<QLabel *>("Y");
    QLabel *ql2=findChild<QLabel *>("U");
    QLabel *ql3=findChild<QLabel *>("V");
    
    switch(YUV_num){
        case (0):{
            ql1->setPixmap(QPixmap::fromImage(jimage.Y));
            ql2->setPixmap(QPixmap::fromImage(jimage.CB));
            ql3->setPixmap(QPixmap::fromImage(jimage.CR));
        }break;
        case (1):{
            ql1->setPixmap(QPixmap::fromImage(jimage.DCT_Y));
            ql2->setPixmap(QPixmap::fromImage(jimage.DCT_U));
            ql3->setPixmap(QPixmap::fromImage(jimage.DCT_V));
        }break;
        case (2):{
            ql1->setPixmap(QPixmap::fromImage(jimage.Quan_Y));
            ql2->setPixmap(QPixmap::fromImage(jimage.Quan_U));
            ql3->setPixmap(QPixmap::fromImage(jimage.Quan_V));
        }break;
        case (3):{
            ql1->setPixmap(QPixmap::fromImage(jimage.DQY));
            ql2->setPixmap(QPixmap::fromImage(jimage.DQU));
            ql3->setPixmap(QPixmap::fromImage(jimage.DQV));
        }break;
        case (4):{
            ql1->setPixmap(QPixmap::fromImage(jimage.DDCTY));
            ql2->setPixmap(QPixmap::fromImage(jimage.DDCTU));
            ql3->setPixmap(QPixmap::fromImage(jimage.DDCTV));
        }break;
    }
}

void MainWindow::on_ycbyr_clicked()
{
    YUV_num=0;
    UpdateYUV();
}

void MainWindow::on_dct_clicked()
{
    YUV_num=1;
    UpdateYUV();
}

void MainWindow::on_quantization_clicked()
{
    YUV_num=2;
    UpdateYUV();
}

void MainWindow::on_dquan_clicked()
{
    YUV_num=3;
    UpdateYUV();
}

void MainWindow::on_idct_clicked()
{
    YUV_num=4;
    UpdateYUV();
}
