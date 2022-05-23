#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <thread>
#include <time.h>
#include <QDebug>
#include <QMessageBox>
#include <qlistwidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Slow_Player");
    preview = NULL;
    preview = new VideoWidget(this);
    preview->resize(160,90);
    preview->move(0, 0);
    preview->setVisible(false);

    qRegisterMetaType<VideoPlayer::VideoSwsSpec>("VideoSwsSpec&");
    _player = new VideoPlayer();
    preview_player = new VideoPlayer();

    connect(_player, &VideoPlayer::stateChanged,
            this, &MainWindow::onPlayerStateChanged);
    connect(_player, &VideoPlayer::timeChanged,
            this, &MainWindow::onPlayerTimeChanged);
    connect(_player, &VideoPlayer::initFinished,
            this, &MainWindow::onPlayerInitFinished);
    connect(_player, &VideoPlayer::playFailed,
            this, &MainWindow::onPlayerPlayFailed);
    connect(_player, &VideoPlayer::frameDecoded,
            ui->videoWidget, &VideoWidget::onPlayerFrameDecoded);
    connect(preview_player, &VideoPlayer::frameDecoded,
            preview, &VideoWidget::onPlayerFrameDecoded);
    connect(_player, &VideoPlayer::stateChanged,
            ui->videoWidget, &VideoWidget::onPlayerStateChanged);
    connect(ui->timeSlider, &VideoSlider::clicked,
            this, &MainWindow::onSliderClicked);
    connect(ui->timeSlider, &VideoSlider::preview,
            this, &MainWindow::onSliderMouseFoucs);
    connect(ui->timeSlider,&VideoSlider::mouseleave,
            this,&MainWindow::onMouseLeaveSlider);


    //音量设置
    ui->volumnSlider->setRange(VideoPlayer::Volumn::Min,VideoPlayer::Volumn::Max);
    ui->volumnSlider->setValue(ui->volumnSlider->maximum() >> 1);
}

MainWindow::~MainWindow() {


    delete ui;
    delete _player;
    delete preview_player;
}


void MainWindow::onSliderClicked(VideoSlider *slider) {
    _player->setTime(slider->value());
}
void MainWindow::onSliderMouseFoucs(int seektime,int x)
{
    //qDebug()<<"获取了一次位置";
    preview_player->setTime(seektime);
    preview_player->updateSignal();
    preview->move(x+40, ui->videoWidget->height() - preview->height() + 20);
    preview->show();



}

void MainWindow::onMouseLeaveSlider(){
    preview->hide();
}

void MainWindow::onPlayerPlayFailed(VideoPlayer *player) {
    QMessageBox::critical(nullptr, "提示", "哦豁，出现神秘的错误！");
}

void MainWindow::onPlayerInitFinished(VideoPlayer *player) {
    int duration = player->getDuration();
    ui->timeSlider->setRange(0, duration);
    ui->durationLabel->setText(getTimeText(duration));
}

void MainWindow::onPlayerTimeChanged(VideoPlayer *player) {
    ui->timeSlider->setValue(player->getTime());
}

void MainWindow::onPlayerStateChanged(VideoPlayer *player) {
    VideoPlayer::State state = player->getState();
    if (state == VideoPlayer::Playing) {
        ui->playBtn->setIcon(QIcon(":/new/prefix/pause.png"));
    }else{
        ui->playBtn->setIcon(QIcon(":/new/prefix/play.png"));
    }
    if (state == VideoPlayer::Stopped) {
        //禁止按钮
        ui->playBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->timeSlider->setEnabled(false);
        ui->volumnSlider->setEnabled(false);
        ui->nextMediaBtn->setEnabled(false);
        ui->lastMediaBtn->setEnabled(false);
        ui->speed3sBtn->setEnabled(false);
        ui->back3sBtn->setEnabled(false);
        ui->mutipleSpeed->setEnabled(false);
        ui->muteBtn->setEnabled(false);
        ui->listBtn->setEnabled(false);
        ui->durationLabel->setText(getTimeText(0));
        ui->timeSlider->setValue(0);

        //返回打开文件
        ui->playWidget->setCurrentWidget(ui->openFilePage);
    } else {
        ui->playBtn->setEnabled(true);
        ui->stopBtn->setEnabled(true);
        ui->timeSlider->setEnabled(true);
        ui->volumnSlider->setEnabled(true);
        ui->nextMediaBtn->setEnabled(true);
        ui->lastMediaBtn->setEnabled(true);
        ui->speed3sBtn->setEnabled(true);
        ui->back3sBtn->setEnabled(true);
        ui->mutipleSpeed->setEnabled(true);
        ui->muteBtn->setEnabled(true);
        ui->listBtn->setEnabled(true);

        //进入播放洁面（先调整界面再进入不会有闪烁）

        ui->videoWidget->resize(0.8*ui->playWidget->width(),ui->playWidget->height());
        ui->listBtn->move(ui->videoWidget->width() - ui->listBtn->width(),ui->videoWidget->height() * 0.5);

        ui->addFileBtn->move(ui->videoWidget->width(),0);
        ui->addFolderBtn->move(ui->videoWidget->width()  + ui->addFolderBtn->width(),0);
        ui->clearListBtn->move(ui->videoWidget->width()  + 2*ui->addFolderBtn->width(),0);
        ui->fileList->resize(ui->playWidget->width() - ui->videoWidget->width(), ui->playWidget->height() - ui->addFileBtn->height());
        ui->fileList->move(ui->videoWidget->width(), ui->addFileBtn->height());

        ui->playWidget->setCurrentWidget(ui->videoPage);



    }
}

void MainWindow::on_stopBtn_clicked() {
    _player->stop();
    preview_player->stopwithSignal();
}

void MainWindow::on_openFileBtn_clicked() {
    QString filePath = QFileDialog::getOpenFileName(  this,
                                                      "选择多媒体文件", //窗口左上角显示
                                                      "/home", //初始路径
                                                      "多媒体文件 (*.mp4 *.avi *.mkv *.mp3 *.aac *.mov *.ts)"
                                                      );


    if(!file.contains(filePath))//不与已有文件重复的情况下
    {
        QFileInfo temp;
        temp.setFile(filePath);
        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::UserRole,temp.absoluteFilePath());
        item->setText(temp.fileName());
        ui->fileList->addItem(item);

        file.insert(filePath); //将文件绝对路径加入到集合中

    }

    ui->playWidget->setCurrentWidget(ui->videoPage);


    _player->setFilename(filePath);
    _player->play();
    preview_player->setFilename(filePath);
    preview_player->play_preview();

}

void MainWindow::on_timeSlider_valueChanged(int value) {
    ui->timeLabel->setText(getTimeText(value));
}


void MainWindow::on_volumnSlider_valueChanged(int value) {
    ui->volumnLabel->setText(QString("%1").arg(value));

    //调整音量时解除静音
    if(_player->isMute())
    {
        ui->muteBtn->setIcon(QIcon(":/new/prefix/aloud.png"));
        _player->setMute(false);
    }

    _player->setVolumn(value);
}

void MainWindow::on_playBtn_clicked() {
    VideoPlayer::State state = _player->getState();
    if (state == VideoPlayer::Playing) {
        _player->pause();
    } else {
        _player->play();
    }
}

void MainWindow::on_listBtn_clicked()
{
    if(!ui->fileList->isHidden())//播放列表为打开状态
    {

        ui->listBtn->move((ui->playWidget->width() - ui->listBtn->width()),0.5*ui->playWidget->height() - ui->listBtn->height());
        ui->fileList->hide();
        ui->addFileBtn->hide();
        ui->addFolderBtn->hide();
        ui->clearListBtn->hide();

        ui->videoWidget->resize(ui->playWidget->width(),ui->playWidget->height());

        ui->listBtn->setIcon(QIcon(":/new/prefix/showList.png"));
    }
    else
    {
        ui->videoWidget->resize(0.8*ui->playWidget->width(),ui->playWidget->height());
        ui->listBtn->move(ui->videoWidget->width() - ui->listBtn->width(),ui->videoWidget->height() * 0.5);

        ui->addFileBtn->move(ui->videoWidget->width(),0);
        ui->addFolderBtn->move(ui->videoWidget->width()  + ui->addFolderBtn->width(),0);
        ui->clearListBtn->move(ui->videoWidget->width()  + 2*ui->addFolderBtn->width(),0);
        ui->fileList->resize(ui->playWidget->width() - ui->videoWidget->width(), ui->playWidget->height() - ui->addFileBtn->height());
        ui->fileList->move(ui->videoWidget->width(), ui->addFileBtn->height());

        ui->fileList->show();
        ui->addFileBtn->show();
        ui->addFolderBtn->show();
        ui->clearListBtn->show();

        ui->listBtn->setIcon(QIcon(":/new/prefix/hideList.png"));

    }
}

void MainWindow::on_fileList_itemDoubleClicked(QListWidgetItem *item)
{
    //点击播放列表时若正在播放则清楚播放帧，重新打开文件
    VideoPlayer::State state = _player->getState();
    if (state == VideoPlayer::Playing)
    {
        _player->stop();
        preview_player->stopwithSignal();
    }

    QString fileAbsolutePath = item->data(Qt::UserRole).toString();

    _player->setFilename(fileAbsolutePath);
    _player->play();
    preview_player->setFilename(fileAbsolutePath);
    preview_player->play_preview();
}

QString MainWindow::getTimeText(int value) {
    //获取XX：XX：XX格式的时间文本
    QLatin1Char fill = QLatin1Char('0');
    return QString("%1:%2:%3")
           .arg(value/3600,2,10,fill)
           .arg((value/60)%60,2,10,fill)
           .arg(value%60,2,10,fill);
}

void MainWindow::on_muteBtn_clicked() {
    if (_player->isMute()) {
        _player->setMute(false);
        ui->muteBtn->setIcon(QIcon(":/new/prefix/mute.png"));
    } else {
        _player->setMute(true);
        ui->muteBtn->setIcon(QIcon(":/new/prefix/aloud.png"));
    }
}


void MainWindow::on_openDirBtn_clicked()
{

    //获取打开的文件夹
    QString filename = QFileDialog::getExistingDirectory(this,
                                                         "选择文件夹",   //窗口左上角显示
                                                         "/home"       //初始路径
                                                         );

    QDir *dir=new QDir(filename);

    //设置目的文件格式
    QStringList filter;
    filter << QString("*.mp4") << QString("*.avi")
           << QString("*.mkv") << QString("*.mp3")
           << QString("*.aac") << QString("*.mov")
           << QString("*.ts");
    dir->setNameFilters(filter);

    QList fileInfo = dir->entryInfoList(QDir::Files | QDir::CaseSensitive);//过滤条件为只限文件并区分大小写
    for(int i = 0;i < fileInfo.count(); i++)
    {
        if(!file.contains(fileInfo.at(i).absoluteFilePath()))//不与已有文件重复的情况下
        {
            file.insert(fileInfo.at(i).absoluteFilePath());//加入到集合中

            QListWidgetItem *item = new QListWidgetItem;
            item->setData(Qt::UserRole,fileInfo.at(i).absoluteFilePath());
            item->setText(fileInfo.at(i).fileName());
            ui->fileList->addItem(item);        //将读取到的文件名同步到播放列表中
        }
    }
    delete dir;

    if(fileInfo.count() == 0)
    {
        ui->playBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->timeSlider->setEnabled(false);
        ui->volumnSlider->setEnabled(false);
        ui->nextMediaBtn->setEnabled(false);
        ui->lastMediaBtn->setEnabled(false);
        ui->speed3sBtn->setEnabled(false);
        ui->back3sBtn->setEnabled(false);
        ui->mutipleSpeed->setEnabled(false);
        ui->muteBtn->setEnabled(false);
        ui->listBtn->setEnabled(false);


        ui->durationLabel->setText(getTimeText(0));
        ui->timeSlider->setValue(0);
        ui->videoWidget->resize(0.8*ui->playWidget->width(),ui->playWidget->height());
        ui->listBtn->move(ui->videoWidget->width() - ui->listBtn->width(),ui->videoWidget->height() * 0.5);
        ui->addFileBtn->move(ui->videoWidget->width(),0);
        ui->addFolderBtn->move(ui->videoWidget->width()  + ui->addFolderBtn->width(),0);
        ui->clearListBtn->move(ui->videoWidget->width()  + 2*ui->addFolderBtn->width(),0);
        ui->fileList->resize(ui->playWidget->width() - ui->videoWidget->width(), ui->playWidget->height() - ui->addFileBtn->height());
        ui->fileList->move(ui->videoWidget->width(), ui->addFileBtn->height());
        ui->listBtn->setIcon(QIcon(":/new/prefix/hideList.png"));
        ui->playWidget->setCurrentWidget(ui->videoPage);
    }
}


void MainWindow::on_nextMediaBtn_clicked()
{

}


void MainWindow::on_lastMediaBtn_clicked()
{

}


void MainWindow::on_addFileBtn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(  this,
                                                      "选择多媒体文件", //窗口左上角显示
                                                      "/home", //初始路径
                                                      "多媒体文件 (*.mp4 *.avi *.mkv *.mp3 *.aac *.mov *.ts)"
                                                      );

    if(!file.contains(filePath))//不与已有文件重复的情况下
    {
        QFileInfo temp;
        temp.setFile(filePath);
        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::UserRole,temp.absoluteFilePath());
        item->setText(temp.fileName());
        ui->fileList->addItem(item);

        file.insert(filePath); //将文件绝对路径加入到集合中
    }

    ui->playWidget->setCurrentWidget(ui->videoPage);


    _player->setFilename(filePath);
    _player->play();
    preview_player->setFilename(filePath);
    preview_player->play_preview();
}


void MainWindow::on_addFolderBtn_clicked()
{
    //获取打开的文件夹
    QString filename = QFileDialog::getExistingDirectory(this,
                                                         "选择文件夹",   //窗口左上角显示
                                                         "/home"       //初始路径
                                                         );
    QDir *dir=new QDir(filename);

    //设置目的文件格式
    QStringList filter;
    filter << QString("*.mp4") << QString("*.avi")
           << QString("*.mkv") << QString("*.mp3")
           << QString("*.aac") << QString("*.mov")
           << QString("*.ts");
    dir->setNameFilters(filter);

    QList fileInfo = dir->entryInfoList(QDir::Files | QDir::CaseSensitive);//过滤条件为只限文件并区分大小写
    for(int i = 0;i < fileInfo.count(); i++)
    {
        if(!file.contains(fileInfo.at(i).absoluteFilePath()))//不与已有文件重复的情况下
        {
            file.insert(fileInfo.at(i).absoluteFilePath());//加入到集合中

            QListWidgetItem *item = new QListWidgetItem;
            item->setData(Qt::UserRole,fileInfo.at(i).absoluteFilePath());
            item->setText(fileInfo.at(i).fileName());
            ui->fileList->addItem(item);        //将读取到的文件名同步到播放列表中
        }
    }
    delete dir;

    ui->playWidget->setCurrentWidget(ui->videoPage);

    //隐藏播放列表时继续导入文件夹，则导入后打开播放列表
    if(ui->fileList->isHidden())
    {
        ui->fileList->show();
        ui->listBtn->setText("隐藏播放列表");
        ui->fileList->resize(0.2*ui->playWidget->width(),ui->playWidget->height());
        ui->fileList->move(0.8*ui->playWidget->width(),0);
        ui->videoWidget->resize(0.8*ui->playWidget->width(),ui->playWidget->height());
    }
}


void MainWindow::on_clearListBtn_clicked()
{

}

