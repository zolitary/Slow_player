#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videowidget.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->volumnSlider->setValue(50);
    //播放器
    _player=new VideoPlayer();

    connect(_player,&VideoPlayer::stateChanged,
            this,&MainWindow::onPlayerStateChanged);
    connect(_player,&VideoPlayer::initFinished,
            this,&MainWindow::onPlayerInitFinished);
    connect(_player, &VideoPlayer::playFailed,
            this, &MainWindow::onPlayerPlayFailed);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _player;
}

void MainWindow::onPlayerPlayFailed(VideoPlayer *player) {
    QMessageBox::critical(nullptr, "提示", "哦豁，播放失败！");
}


void MainWindow::on_openfileBtn_clicked()
{
    QString filename=(QFileDialog::getOpenFileName(nullptr,
                                                  "选择打开多媒体文件",
                                                  "/",
                                                  "视频文件 (*.mp4 *.avi);;音频文件 (*.mp3 *.aac)").toUtf8().data());
    qDebug()<<filename;
    //qDebug()<<"fhjghjkgjjgjkkj"<<filename.toUtf8().data();
    if(filename.isEmpty()) return;

    //播放
    _player->setFilename(filename);
    _player->play();




}

void MainWindow::onPlayerStateChanged(VideoPlayer *player){
    VideoPlayer::State state=player->getState();
    if(state==VideoPlayer::Playing){
        ui->playBtn->setText("暂停");
    }else{
        ui->playBtn->setText("播放");
    }

    if(state==VideoPlayer::Nosource){
        ui->playBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->silenceBtn->setEnabled(false);
        ui->volumnSlider->setEnabled(false);
        ui->currentSlider->setEnabled(false);
        ui->currentSlider->setValue(0);
        ui->durationLable->setText(getTimeText(0));
        ui->playWidget->setCurrentWidget(ui->openFilePage);
    }else{
        ui->playBtn->setEnabled(true);
        ui->stopBtn->setEnabled(true);
        ui->silenceBtn->setEnabled(true);
        ui->volumnSlider->setEnabled(true);
        ui->currentSlider->setEnabled(true);
        ui->playWidget->setCurrentWidget(ui->videoPage);
    }
}

void MainWindow::onPlayerInitFinished(VideoPlayer *player) {
    int duration = player->getDuration();
    ui->currentSlider->setRange(0, duration);

    ui->durationLable->setText(getTimeText(duration));
}

void MainWindow::on_currentSlider_valueChanged(int value) {
    ui->currentLable->setText(getTimeText(value));
}


void MainWindow::on_volumnSlider_valueChanged(int value)
{
    ui->volumnLable->setText(QString("%1").arg(value));
}


void MainWindow::on_stopBtn_clicked()
{
    _player->stop();
}


void MainWindow::on_playBtn_clicked()
{
    VideoPlayer::State state=_player->getState();
    if(state==VideoPlayer::Playing){
        ui->playBtn->setText("暂停");
    }else{
        ui->playBtn->setText("播放");
    }
}

QString MainWindow::getTimeText(int value) {

    QLatin1Char fill = QLatin1Char('0');
    return QString("%1:%2:%3")
           .arg(value / 3600, 2, 10, fill)
           .arg((value / 60) % 60, 2, 10, fill)
           .arg(value % 60, 2, 10, fill);
}
