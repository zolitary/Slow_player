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
        ui->playBtn->setText("暂停");
    }else{
        ui->playBtn->setText("播放");
    }
    if (state == VideoPlayer::Stopped) {
        //禁止按钮
        ui->playBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->timeSlider->setEnabled(false);
        ui->volumnSlider->setEnabled(false);
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
        ui->muteBtn->setEnabled(true);
        ui->listBtn->setEnabled(true);
        //进入播放洁面
        ui->playWidget->setCurrentWidget(ui->videoPage);

        ui->fileList->resize(0.2*ui->playWidget->width(),ui->playWidget->height());
        ui->fileList->move(0.8*ui->playWidget->width(),0);
        ui->videoWidget->resize(0.8*ui->playWidget->width(),ui->playWidget->height());
    }
}

void MainWindow::on_stopBtn_clicked() {
    _player->stop();
    preview_player->stopwithSignal();
}

void MainWindow::on_openFileBtn_clicked() {

    //打开新的文件夹前清空已读入的列表
    if(ui->fileList->count() != 0)
    {
        ui->fileList->clear();
    }

    //获取打开的文件夹
    QString filename = QFileDialog::getExistingDirectory();
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
        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::UserRole,fileInfo.at(i).absoluteFilePath());
        item->setText(fileInfo.at(i).fileName());
        ui->fileList->addItem(item);        //将读取到的文件名同步到视图中
    }

    ui->fileList->setBackgroundRole(QPalette::Highlight);
    ui->fileList->update(); //更新视图

    ui->playWidget->setCurrentWidget(ui->videoPage);
    ui->fileList->show();

}

void MainWindow::on_timeSlider_valueChanged(int value) {
    ui->timeLabel->setText(getTimeText(value));
}


void MainWindow::on_volumnSlider_valueChanged(int value) {
    ui->volumnLabel->setText(QString("%1").arg(value));
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
        ui->fileList->hide();
        ui->listBtn->setText("显示播放列表");
        ui->videoWidget->resize(ui->playWidget->width(),ui->playWidget->height());
    }
    else
    {
        ui->fileList->show();
        ui->listBtn->setText("隐藏播放列表");
        ui->fileList->resize(0.2*ui->playWidget->width(),ui->playWidget->height());
        ui->fileList->move(0.8*ui->playWidget->width(),0);
        ui->videoWidget->resize(0.8*ui->playWidget->width(),ui->playWidget->height());
    }
}

void MainWindow::on_fileList_itemDoubleClicked(QListWidgetItem *item)
{
    //点击播放列表时若正在播放则清楚播放帧，重新打开文件
    VideoPlayer::State state = _player->getState();
    if (state == VideoPlayer::Playing)
    {
        _player->stop();
        preview_player->stop();
    }

    //data中的是绝对路径，text是文件名
    QString fileAbolutePath = item->data(Qt::UserRole).toString(),
            fileName = item->text();


    _player->setFilename(fileAbolutePath);
    _player->play();
    preview_player->setFilename(fileAbolutePath);
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
        ui->muteBtn->setText("静音");
    } else {
        _player->setMute(true);
        ui->muteBtn->setText("声音");
    }
}

