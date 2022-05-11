#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videowidget.h"

#include <QFileDialog>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openfileBtn_clicked()
{
    QString filename=QFileDialog::getOpenFileName(nullptr,
                                                  "选择打开多媒体文件",
                                                  "/",
                                                  "视频文件 (*.mp4 *.avi);;音频文件 (*.mp3 *.aac)");
    qDebug()<<filename;
}

