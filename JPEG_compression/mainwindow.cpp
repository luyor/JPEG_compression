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

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
      tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
    QImage image;
    if (!image.load(fileName)) QDialog::tr("cannot read image file");
    QTextStream(stdout) <<fileName<<endl;
    
    QLabel *gv=findChild<QLabel *>("origin");
    gv->setPixmap(QPixmap::fromImage(image));
}

