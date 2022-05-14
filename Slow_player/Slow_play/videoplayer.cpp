#include "videoplayer.h"
#include <thread>
#include <QDebug>

#define AUDIO_MAX_PKT_SIZE 1000
#define VIDEO_MAX_PKT_SIZE 500


VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent) {
    //初始化SDL Audio系统
    if (SDL_Init(SDL_INIT_AUDIO)) {
        qDebug() << "SDL_Init error" << SDL_GetError();
        emit playFailed(this);
        return;
    }
}

VideoPlayer::~VideoPlayer() {
    disconnect();
    stop();
    SDL_Quit();
}

void VideoPlayer::play() {
    if (_state == Playing) return;
    if (_state == Stopped) {
        std::thread([this]() {
            readFile();
        }).detach();
    } else {
        setState(Playing);
    }
}

void VideoPlayer::pause() {
    if (_state != Playing) return;
    setState(Paused);
}

void VideoPlayer::stop() {
    if (_state == Stopped) return;
    _state = Stopped;
    // 释放资源
    free();
    emit stateChanged(this);
}

bool VideoPlayer::isPlaying() {
    return _state == Playing;
}

VideoPlayer::State VideoPlayer::getState() {
    return _state;
}

void VideoPlayer::setFilename(QString &filename) {
    const char *name=filename.toStdString().c_str();
    memcpy(_filename,name,strlen(name)+1);
}

int VideoPlayer::getDuration() {
    return _fmtCtx?round(_fmtCtx->duration*av_q2d(AV_TIME_BASE_Q)):0;//时间转化为秒
}

int VideoPlayer::getTime() {
    return round(_aTime);
}

void VideoPlayer::setTime(int seekTime) {
    _seekTime=seekTime;
}

void VideoPlayer::setVolumn(int volumn) {
    _volumn=volumn;
}

int VideoPlayer::getVolumn() {
    return _volumn;
}

void VideoPlayer::setMute(bool mute) {
    _mute=mute;
}

bool VideoPlayer::isMute() {
    return _mute;
}

void VideoPlayer::readFile() {

    int ret = 0;
    //创建解封装上下文、打开文件
    ret = avformat_open_input(&_fmtCtx, _filename, nullptr, nullptr);
    END(avformat_open_input);

    //检索流信息
    ret = avformat_find_stream_info(_fmtCtx, nullptr);
    END(avformat_find_stream_info);

    //打印流信息
    av_dump_format(_fmtCtx, 0, _filename, 0);
    fflush(stderr);

    //初始化音频信息
    _hasAudio = initAudioInfo() >= 0;
    //初始化视频信息
    _hasVideo = initVideoInfo() >= 0;
    if (!_hasAudio && !_hasVideo) {
        fataError();
        return;
    }

    emit initFinished(this);

    setState(Playing);

    //音频解码
    SDL_PauseAudio(0);

    //视频解码
    std::thread([this]() {
        decodeVideo();
    }).detach();

    //从输入文件中读取数据
    AVPacket pkt;
    while (_state != Stopped) {
        //处理seek操作
        if (_seekTime >= 0) {
            int streamIdx;
            //选择使用流作为时间基准 seek
#if 1
            if (_hasAudio) {
                streamIdx = _aStream->index;
            }else{
                streamIdx = _vStream->index;
            }
#endif
#if 0
            if (_hasVideo) {
                streamIdx = _vStream->index;
            }else{
                streamIdx = _aStream->index;
            }
#endif
            AVRational timeBase = _fmtCtx->streams[streamIdx]->time_base;
            int64_t ts = _seekTime / av_q2d(timeBase);
            ret = av_seek_frame(_fmtCtx, streamIdx, ts, AVSEEK_FLAG_BACKWARD);
            if (ret < 0) { // seek失败
                qDebug() << "seek失败" << _seekTime << ts << streamIdx;
                _seekTime = -1;
            } else {
                qDebug() << "seek成功" << _seekTime << ts << streamIdx;
                _vSeekTime = _seekTime;
                _aSeekTime = _seekTime;
                _seekTime = -1;
                // 恢复时钟
                _aTime = 0;
                _vTime = 0;
                // 清空数据包
                clearAudioPktList();
                clearVideoPktList();
            }
        }

        int vSize = _vPktList.size();
        int aSize = _aPktList.size();

        if (vSize >= VIDEO_MAX_PKT_SIZE ||
                aSize >= AUDIO_MAX_PKT_SIZE) {
            continue;
        }

        ret = av_read_frame(_fmtCtx, &pkt);
        if (ret == 0) {
            if (pkt.stream_index == _aStream->index) {
                addAudioPkt(pkt);
            } else if (pkt.stream_index == _vStream->index) {
                if(_vPktList.empty()){
                    if(!(pkt.flags &AV_PKT_FLAG_KEY)){
                        //continue;
                    }
                }
                addVideoPkt(pkt);
            } else { //非视频、音频包
                av_packet_unref(&pkt);
            }
        } else if (ret == AVERROR_EOF) { // 读到了文件的尾部
            if (vSize == 0 && aSize == 0) {
                //等待播放完成 包队列取完
                _fmtCtxCanFree = true;
                break;
            }
        } else {
            ERROR_BUF;
            qDebug() << "av_read_frame error" << errbuf;
            continue;
        }
    }
    if (_fmtCtxCanFree) {
        stop();
    }else{
        _fmtCtxCanFree = true;
    }
}

// 初始化解码器
int VideoPlayer::initDecoder(AVCodecContext **decodeCtx,
                             AVStream **stream,
                             AVMediaType type) {
    // 根据type寻找最合适的流信息 返回流索引
    int ret = av_find_best_stream(_fmtCtx, type, -1, -1, nullptr, 0);
    RET(av_find_best_stream);

    // 检验流
    int streamIdx = ret;
    *stream = _fmtCtx->streams[streamIdx];
    if (!*stream) {
        qDebug() << "stream is empty";
        return -1;
    }

    // 为当前流找到合适的解码器
    const AVCodec *decoder = avcodec_find_decoder((*stream)->codecpar->codec_id);
    if (!decoder) {
        qDebug() << "decoder not found" << (*stream)->codecpar->codec_id;
        return -1;
    }

    // 初始化解码上下文
    *decodeCtx = avcodec_alloc_context3(decoder);
    if (!decodeCtx) {
        qDebug() << "avcodec_alloc_context3 error";
        return -1;
    }

    // 从流中拷贝参数到解码上下文中
    ret = avcodec_parameters_to_context(*decodeCtx, (*stream)->codecpar);
    RET(avcodec_parameters_to_context);

    // 打开解码器
    ret = avcodec_open2(*decodeCtx, decoder, nullptr);
    RET(avcodec_open2);

    return 0;
}

void VideoPlayer::setState(State state) {
    if (state == _state) return;
    _state = state;
    emit stateChanged(this);
}

void VideoPlayer::free() {
    while (_hasAudio && !_aCanFree);
    while (_hasVideo && !_vCanFree);
    while (!_fmtCtxCanFree);
    avformat_close_input(&_fmtCtx);
    _fmtCtxCanFree = false;
    _seekTime = -1;
    freeAudio();
    freeVideo();
}

void VideoPlayer::fataError() {
    _state = Playing;
    stop();
    emit playFailed(this);
}

