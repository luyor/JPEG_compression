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
    
    void on_nonuniform_clicked();
    
    void on_constant_clicked();
    
    void on_submit_btn_clicked();
    
    void on_quality_editingFinished();
    
    void on_ycbyr_clicked();
    
    void on_dct_clicked();
    
    void on_quantization_clicked();
    
    void on_dquan_clicked();
    
    void on_idct_clicked();
    
private:
    Ui::MainWindow *ui;
    JImage jimage;
};

#endif // MAINWINDOW_H
