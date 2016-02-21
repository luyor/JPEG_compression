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
    //jimage.UpdateImage();
    
    QLabel *ql=findChild<QLabel *>("origin");
    ql->setPixmap(QPixmap::fromImage(jimage.origin));
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

void MainWindow::on_ycbyr_clicked()
{
    QLabel *ql1=findChild<QLabel *>("Y");
    ql1->setPixmap(QPixmap::fromImage(jimage.Y));
    QLabel *ql2=findChild<QLabel *>("U");
    ql2->setPixmap(QPixmap::fromImage(jimage.CB));
    QLabel *ql3=findChild<QLabel *>("V");
    ql3->setPixmap(QPixmap::fromImage(jimage.CR));
}

void MainWindow::on_dct_clicked()
{
    QLabel *ql1=findChild<QLabel *>("Y");
    ql1->setPixmap(QPixmap::fromImage(jimage.DCT_Y));
    QLabel *ql2=findChild<QLabel *>("U");
    ql2->setPixmap(QPixmap::fromImage(jimage.DCT_U));
    QLabel *ql3=findChild<QLabel *>("V");
    ql3->setPixmap(QPixmap::fromImage(jimage.DCT_V));
}

void MainWindow::on_quantization_clicked()
{
    QLabel *ql1=findChild<QLabel *>("Y");
    ql1->setPixmap(QPixmap::fromImage(jimage.Quan_Y));
    QLabel *ql2=findChild<QLabel *>("U");
    ql2->setPixmap(QPixmap::fromImage(jimage.Quan_U));
    QLabel *ql3=findChild<QLabel *>("V");
    ql3->setPixmap(QPixmap::fromImage(jimage.Quan_V));
}

void MainWindow::on_dquan_clicked()
{
    QLabel *ql1=findChild<QLabel *>("Y");
    ql1->setPixmap(QPixmap::fromImage(jimage.DQY));
    QLabel *ql2=findChild<QLabel *>("U");
    ql2->setPixmap(QPixmap::fromImage(jimage.DQU));
    QLabel *ql3=findChild<QLabel *>("V");
    ql3->setPixmap(QPixmap::fromImage(jimage.DQV));
}

void MainWindow::on_idct_clicked()
{
    QLabel *ql1=findChild<QLabel *>("Y");
    ql1->setPixmap(QPixmap::fromImage(jimage.DDCTY));
    QLabel *ql2=findChild<QLabel *>("U");
    ql2->setPixmap(QPixmap::fromImage(jimage.DDCTU));
    QLabel *ql3=findChild<QLabel *>("V");
    ql3->setPixmap(QPixmap::fromImage(jimage.DDCTV));
}
