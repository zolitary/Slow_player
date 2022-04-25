#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug("------------------------------------------------------------------------");
    qDebug("%s", avcodec_configuration());
    qDebug("version: %d", avcodec_version());
    qDebug("------------------------------------------------------------------------");
}

MainWindow::~MainWindow()
{
    delete ui;
}

