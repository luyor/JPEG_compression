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

void MainWindow::on_spinBox_editingFinished()
{
    QSpinBox *qsb=findChild<QSpinBox *>("quality");
    jimage.ChangeQuality(qsb->value());
}

void MainWindow::on_constant_clicked()
{
    jimage.ChangeConst(true);
}
