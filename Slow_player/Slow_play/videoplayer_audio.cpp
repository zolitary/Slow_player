#include "videoplayer.h"
#include <QDebug>

int VideoPlayer::initAudioInfo() {
    // 初始化解码器
    int ret=initDecoder(&_aDecodeCtx, &_aStream, AVMEDIA_TYPE_AUDIO);
    RET(initDecoder);

    // 初始化音频重采样
    ret=initSwr();
    RET(initSwr);

    // 初始化SDL
    ret=initSDL();
    RET(initSDL);

    return 0;
}

int VideoPlayer::initSwr() {
    // 重采样输入参数
    _aSwrInSpec.sampleFmt=_aDecodeCtx->sample_fmt;
    _aSwrInSpec.sampleRate=_aDecodeCtx->sample_rate;
    _aSwrInSpec.chLayout=_aDecodeCtx->channel_layout;
    _aSwrInSpec.chs=_aDecodeCtx->channels;

    // 重采样输出参数
    _aSwrOutSpec.sampleFmt=AV_SAMPLE_FMT_S16;
    _aSwrOutSpec.sampleRate=44100;
    _aSwrOutSpec.chLayout=AV_CH_LAYOUT_STEREO;
    _aSwrOutSpec.chs=av_get_channel_layout_nb_channels(_aSwrOutSpec.chLayout);
    _aSwrOutSpec.bytesPerSampleFrame=_aSwrOutSpec.chs
                                       * av_get_bytes_per_sample(_aSwrOutSpec.sampleFmt);

    // 创建重采样上下文
    _aSwrCtx=swr_alloc_set_opts(nullptr,
                                  _aSwrOutSpec.chLayout,_aSwrOutSpec.sampleFmt,_aSwrOutSpec.sampleRate,
                                  _aSwrInSpec.chLayout,_aSwrInSpec.sampleFmt,_aSwrInSpec.sampleRate,
                                  0, nullptr);
    if (!_aSwrCtx) {
        qDebug() << "swr_alloc_set_opts error";
        return -1;
    }

    // 初始化重采样上下文
    int ret=swr_init(_aSwrCtx);
    RET(swr_init);

    // 初始化重采样的输入frame
    _aSwrInFrame=av_frame_alloc();
    if (!_aSwrInFrame) {
        qDebug() << "av_frame_alloc error";
        return -1;
    }

    // 初始化重采样的输出frame
    _aSwrOutFrame=av_frame_alloc();
    if (!_aSwrOutFrame) {
        qDebug() << "av_frame_alloc error";
        return -1;
    }

    // 初始化输出frame data空间
    ret=av_samples_alloc(_aSwrOutFrame->data,
                           _aSwrOutFrame->linesize,
                           _aSwrOutSpec.chs,
                           4096, _aSwrOutSpec.sampleFmt, 1);
    RET(av_samples_alloc);

    return 0;
}

int VideoPlayer::initSDL() {
    SDL_AudioSpec spec;
    spec.freq=_aSwrOutSpec.sampleRate;
    spec.format=AUDIO_S16LSB;
    spec.channels=_aSwrOutSpec.chs;
    spec.samples=512;//音频缓冲区的样本数量
    spec.callback=sdlAudioCallbackFunc;
    spec.userdata=this;

    // 打开音频设备
    if (SDL_OpenAudio(&spec, nullptr)) {
        qDebug() << "SDL_OpenAudio error" << SDL_GetError();
        return -1;
    }

    return 0;
}

void VideoPlayer::addAudioPkt(AVPacket &pkt) {
    _aMutex.lock();
    _aPktList.push_back(pkt);
    _aMutex.signal();
    _aMutex.unlock();
}

void VideoPlayer::clearAudioPktList() {
    _aMutex.lock();
    for (AVPacket &pkt : _aPktList) {
        av_packet_unref(&pkt);
    }
    _aPktList.clear();
    _aMutex.unlock();
}

void VideoPlayer::freeAudio() {
    _aTime=0;
    _aSwrOutIdx=0;
    _aSwrOutSize=0;
    _aStream=nullptr;
    _aCanFree=false;
    _aSeekTime=-1;

    clearAudioPktList();
    avcodec_free_context(&_aDecodeCtx);
    swr_free(&_aSwrCtx);
    av_frame_free(&_aSwrInFrame);
    if (_aSwrOutFrame) {
        av_freep(&_aSwrOutFrame->data[0]);
        av_frame_free(&_aSwrOutFrame);
    }

    // 停止播放
    SDL_PauseAudio(1);
    SDL_CloseAudio();
}

//回调函数
void VideoPlayer::sdlAudioCallbackFunc(void *userdata, Uint8 *stream, int len) {
    VideoPlayer *player=(VideoPlayer *) userdata;
    player->sdlAudioCallback(stream, len);
}

void VideoPlayer::sdlAudioCallback(Uint8 *stream, int len) {
    // 清零
    SDL_memset(stream, 0, len);

    while (len > 0) {
        if (_state == Paused) break;
        if (_state == Stopped) {
            _aCanFree=true;
            break;
        }

        //解码pkt，获取新的PCM数据
        if (_aSwrOutIdx >= _aSwrOutSize) {
            //获取新的包解码出来的PCM
            _aSwrOutSize=decodeAudio();
            _aSwrOutIdx=0;
            //没有解码数据 播放静音数据
            if (_aSwrOutSize <= 0) {
                _aSwrOutSize=1024;
                memset(_aSwrOutFrame->data[0], 0, _aSwrOutSize);
            }
        }

        // 本次需要填充到stream中的PCM数据大小
        int fillLen=_aSwrOutSize - _aSwrOutIdx;
        fillLen=std::min(fillLen, len);

        // 获取当前音量
        int volumn=_mute?0:((_volumn*1.0/Max)*SDL_MIX_MAXVOLUME);

        // 填充SDL缓冲区
        SDL_MixAudio(stream,
                     _aSwrOutFrame->data[0]+_aSwrOutIdx,
                     fillLen, volumn);

        // 移动偏移量
        len -= fillLen;
        stream += fillLen;
        _aSwrOutIdx += fillLen;
    }
}

int VideoPlayer::decodeAudio() {
    _aMutex.lock();

    if (_aPktList.empty()) {
        _aMutex.unlock();
        return 0;
    }

    //取出数据包
    AVPacket pkt=_aPktList.front();
    _aPktList.pop_front();
    _aMutex.unlock();

    // 保存音频时钟
    if (pkt.pts != AV_NOPTS_VALUE) {//音频时钟无效判断
        _aTime=av_q2d(_aStream->time_base) * pkt.pts;//获取包时间
        // 通知外界：播放时间点发生了改变
        emit timeChanged(this);
    }

    if (_aSeekTime >= 0) {
        if (_aTime < _aSeekTime) {
            // 释放pkt
            av_packet_unref(&pkt);
            return 0;
        } else {
            _aSeekTime=-1;
        }
    }

    // 发送压缩数据到解码器
    int ret=avcodec_send_packet(_aDecodeCtx, &pkt);
    av_packet_unref(&pkt);
    RET(avcodec_send_packet);

    // 获取解码后的数据
    ret=avcodec_receive_frame(_aDecodeCtx, _aSwrInFrame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
        return 0;
    } else RET(avcodec_receive_frame);

    //解码后数据送入过滤器

    switch(currentSpeedIndex){
    case 1:
        av_buffersrc_add_frame(srcFilterCtx_1,_aSwrInFrame);
        av_buffersink_get_frame(sinkFilterCtx_1,_aSwrInFrame);
        break;
    case 2:
        av_buffersrc_add_frame(srcFilterCtx_2,_aSwrInFrame);
        av_buffersink_get_frame(sinkFilterCtx_2,_aSwrInFrame);
        break;
    case 3:
        av_buffersrc_add_frame(srcFilterCtx_3,_aSwrInFrame);
        av_buffersink_get_frame(sinkFilterCtx_3,_aSwrInFrame);
        break;
    case 4:
        av_buffersrc_add_frame(srcFilterCtx_4,_aSwrInFrame);
        av_buffersink_get_frame(sinkFilterCtx_4,_aSwrInFrame);
        break;
    default:
        av_buffersrc_add_frame(srcFilterCtx_2,_aSwrInFrame);
        av_buffersink_get_frame(sinkFilterCtx_2,_aSwrInFrame);
    };


    // 重采样输出的样本数
    //a*b/c 向上取整
    int outSamples=av_rescale_rnd(_aSwrOutSpec.sampleRate,
                                    _aSwrInFrame->nb_samples,
                                    _aSwrInSpec.sampleRate, AV_ROUND_UP);

    //重采样
    //return number of samples output per channel, negative value on error
    ret=swr_convert(_aSwrCtx,
                      _aSwrOutFrame->data,
                      outSamples,
                      (const uint8_t **) _aSwrInFrame->data,
                      _aSwrInFrame->nb_samples);
    RET(swr_convert);

    return ret * _aSwrOutSpec.bytesPerSampleFrame;
}
