#include "informationwindow.h"
#include "ui_informationwindow.h"

informationWindow::informationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::informationWindow)
{
    ui->setupUi(this);
}

informationWindow::~informationWindow()
{
    delete ui;
}

void informationWindow::updateinformation(AVFormatContext *_updatefmtCtx,char *_filename){
    int audioindex=-1;
    int videoindex = -1;
    for (int i = 0; i < _updatefmtCtx->nb_streams; i++) {
        if (_updatefmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
        }
        else if (_updatefmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioindex = i;
        }
    }
    _fmtCtx=_updatefmtCtx;
    ui->fileNamelabel->setText("名称："+QString(_filename));
    ui->achannelslabel->setText("音频声道数："+(audioindex==-1?"无法获取音频信息":QString::number(_updatefmtCtx->streams[audioindex]->codecpar->channels)));
    ui->asampleRatelabel->setText("音频采样率："+(audioindex==-1?"无法获取音频信息":QString::number(_updatefmtCtx->streams[audioindex]->codecpar->sample_rate)));
    ui->bitRatelabel->setText("码率："+QString::number(_updatefmtCtx->bit_rate));
    ui->durationlabel->setText("总时长："+QString::number(round(_updatefmtCtx->duration*av_q2d(AV_TIME_BASE_Q)))+"秒");
    ui->pixFmtlabel->setText("视频像素格式"+(videoindex==-1?"无法获取视频信息":QString::number(_updatefmtCtx->streams[videoindex]->codecpar->format)));
    ui->namelabel->setText("封装格式名称："+QString(_updatefmtCtx->iformat->name));

}
