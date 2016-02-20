#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>

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
    if (!image.load(fileName)) QDialog::tr("cannot read image file");
    //QTextStream(stdout) <<fileName<<endl;
    jimage.ChangeImage(image);
    jimage.UpdateImage();
    
    QLabel *gv=findChild<QLabel *>("origin");
    gv->setPixmap(QPixmap::fromImage(jimage.DCT_Y));
}

void MainWindow::on_nonuniform_clicked()
{
    
}

void MainWindow::on_submit_btn_clicked()
{
    
}

void MainWindow::on_spinBox_editingFinished()
{
    
}

void MainWindow::on_constant_clicked()
{
    
}
