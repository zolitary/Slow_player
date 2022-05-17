#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <thread>
#include <time.h>
#include <QDebug>
#include <QMessageBox>

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

    //音量设置
    ui->volumnSlider->setRange(VideoPlayer::Volumn::Min,VideoPlayer::Volumn::Max);
    ui->volumnSlider->setValue(ui->volumnSlider->maximum() >> 1);

    std::thread([this](){
        clock_t start=clock();
        while( preview!=NULL )
        {
            if(clock()-start > 3000)
            {
                this->preview->hide();
                start = clock();
            }
        }

    }).detach();
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
    qDebug()<<"获取了一次位置";
    preview->move(x, ui->videoWidget->height() - preview->height() + 20);
    preview->show();
    preview_player->setTime(seektime);


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
        //进入播放洁面
        ui->playWidget->setCurrentWidget(ui->videoPage);
    }
}

void MainWindow::on_stopBtn_clicked() {
    _player->stop();
    preview_player->stop();
}

void MainWindow::on_openFileBtn_clicked() {
    QString filename = QFileDialog::getOpenFileName(nullptr,
                       "选择多媒体文件",
                       "/",
                       "多媒体文件 (*.mp4 *.avi *.mkv *.mp3 *.aac *.mov *.ts)");
    if (filename.isEmpty()) return;
    _player->setFilename(filename);
    _player->play();
    preview_player->setFilename(filename);
    preview_player->play_preview();
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
