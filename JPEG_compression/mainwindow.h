#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <jimage.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_open_btn_clicked();
    
    void on_spinBox_editingFinished();
    
    void on_nonuniform_clicked();
    
    void on_constant_clicked();
    
    void on_submit_btn_clicked();
    
private:
    Ui::MainWindow *ui;
    JImage jimage;
};

#endif // MAINWINDOW_H
