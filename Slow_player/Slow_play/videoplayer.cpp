#include "videoplayer.h"
#include "videoslider.h"
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

void VideoPlayer::play_preview() {
    if (_state == Playing) return;
    if (_state == Stopped) {
        std::thread([this]() {
            startPreview();
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

void VideoPlayer::stopwithSignal() {
    if (_state == Stopped) return;
    _state = Stopped;
    updateSignal();
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

    //初始化倍数过滤器
    initFilter(&graph_1, &srcFilterCtx_1, &sinkFilterCtx_1, "0.5");
    initFilter(&graph_2, &srcFilterCtx_2, &sinkFilterCtx_2, "1.0");
    initFilter(&graph_3, &srcFilterCtx_3, &sinkFilterCtx_3, "1.5");
    initFilter(&graph_4, &srcFilterCtx_4, &sinkFilterCtx_4, "2.0");

    emit initFinished(this);

    setState(Playing);

    //音频解码
    SDL_PauseAudio(0);

    //视频解码
    std::thread([this]() {
        decodeVideo();
    }).detach();

    //从输入文件中读取数据
    AVPacket pkt,keypkt,temp;
    while (_state != Stopped) {
        //处理seek操作
        if (_seekTime >= 0) {
            int streamIdx;
            //选择使用流作为时间基准 seek
#if 0
            if (_hasAudio) {
                streamIdx = _aStream->index;
            }else{
                streamIdx = _vStream->index;
            }
#endif
//            if (_hasVideo) {
//                streamIdx = _vStream->index;
//            }else{
//                streamIdx = _aStream->index;
//            }
            if (_hasAudio) {
                streamIdx = _aStream->index;
            }else{
                streamIdx = _vStream->index;
            }

            //定位到seekTime前20秒
            AVRational timeBase = _fmtCtx->streams[streamIdx]->time_base;
            int64_t ts;
            ts = (_seekTime < 20 ? 0 : _seekTime - 20) / av_q2d(timeBase);

            ret = av_seek_frame(_fmtCtx, streamIdx, ts, AVSEEK_FLAG_BACKWARD);
            if (ret < 0) { // seek失败
                qDebug() << "seek失败" << _seekTime << ts << streamIdx;
                _seekTime = -1;
            } else {            

                // 清空数据包
                clearAudioPktList();
                clearVideoPktList();
                double Time = 0;

                // 获取最近关键帧
                av_read_frame(_fmtCtx, &temp);
                while(1){
                    ret = av_read_frame(_fmtCtx, &keypkt);
                    if (ret == 0) {
                        if (keypkt.stream_index == _vStream->index) {
                            if(_vPktList.empty()){
                                //只需要关键帧
                                if(!(keypkt.flags &AV_PKT_FLAG_KEY)){
                                    continue;
                                }

                                Time = av_q2d(_vStream->time_base) * keypkt.dts;
                                if(Time > _seekTime){
                                    av_packet_unref(&keypkt);
                                    //最近关键帧添加到视频包队列
                                    addVideoPkt(temp);
                                    break;
                                }
                                av_packet_unref(&temp);
                                av_packet_ref(&temp,&keypkt);
                                av_packet_unref(&keypkt);

                            }
                        }
                    }
                }
                Time = av_q2d(_vStream->time_base) * temp.dts;
                //关键帧的时间
                ts = Time / av_q2d(timeBase);
                //重新定位到关键帧
                ret = av_seek_frame(_fmtCtx, streamIdx, ts, AVSEEK_FLAG_BACKWARD);
                if (ret < 0) { // seek失败
                    qDebug() << "seek失败" << Time << ts << streamIdx;
                    _seekTime = -1;
                } else {

                    //将最近关键帧到seekTime里的视频包读入队列
                    while(1){
                        ret = av_read_frame(_fmtCtx, &keypkt);
                        if (ret == 0) {
                            if (keypkt.stream_index == _vStream->index) {
                                Time = av_q2d(_vStream->time_base) * keypkt.dts;
                                if(Time > _seekTime){
                                    av_packet_unref(&keypkt);
                                    break;
                                }
                                addVideoPkt(keypkt);
                            }
                        }
                    }

                    //定位到seekTime
                    ts = _seekTime / av_q2d(timeBase);
                    ret = av_seek_frame(_fmtCtx, streamIdx, ts, AVSEEK_FLAG_BACKWARD);
                    if (ret < 0) { // seek失败
                        qDebug() << "seek失败" << _seekTime << ts << streamIdx;
                        _seekTime = -1;
                    } else {
                        qDebug() << "seek成功" << Time << ts << streamIdx;
                        clearAudioPktList();
                        _vSeekTime = _seekTime;
                        _aSeekTime = _seekTime;
                        _seekTime = -1;
                        // 恢复时钟
                        _aTime = 0;
                        _vTime = 0;

                    }
                }

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

void VideoPlayer::startPreview() {

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

    //抛弃音频
    _hasAudio = false;

    //初始化视频信息
    _hasVideo = initVideoInfo() >= 0;
    if (!_hasAudio && !_hasVideo) {
        fataError();
        return;
    }

    emit initFinished(this);

    setState(Playing);


    //视频解码
    std::thread([this]() {
        decodeVideo();
    }).detach();

    //从输入文件中读取数据
    AVPacket keypkt,temp;
    while (_state != Stopped) {
        _previewMutex.lock();
        _previewMutex.wait();
        _previewMutex.unlock();

        //处理seek操作
        if (_seekTime >= 0) {
            int streamIdx;
            //选择使用流作为时间基准 seek
            if (_hasVideo) {
                streamIdx = _vStream->index;
            }else{
                return;
            }

            //定位到seekTime前20秒
            AVRational timeBase = _fmtCtx->streams[streamIdx]->time_base;
            int64_t ts;
            ts = (_seekTime < 20 ? 0 : _seekTime - 20) / av_q2d(timeBase);

            ret = av_seek_frame(_fmtCtx, streamIdx, ts, AVSEEK_FLAG_BACKWARD);
            if (ret < 0) { // seek失败
                qDebug() << "seek失败" << _seekTime << ts << streamIdx;
                _seekTime = -1;
            } else {

                // 清空数据包
                clearVideoPktList();
                double Time = 0;

                // 获取最近关键帧
                av_read_frame(_fmtCtx, &temp);
                while(1){
                    ret = av_read_frame(_fmtCtx, &keypkt);
                    if (ret == 0) {
                        if (keypkt.stream_index == _vStream->index) {
                            if(_vPktList.empty()){
                                if(!(keypkt.flags &AV_PKT_FLAG_KEY)){
                                    continue;
                                }

                                Time = av_q2d(_vStream->time_base) * keypkt.dts;
                                if(Time > _seekTime){
                                    av_packet_unref(&keypkt);
                                    addVideoPkt(temp);
                                    break;
                                }
                                av_packet_unref(&temp);
                                av_packet_ref(&temp,&keypkt);
                                av_packet_unref(&keypkt);

                            }
                        }
                    }
                }
                Time = av_q2d(_vStream->time_base) * temp.dts;
                ts = Time / av_q2d(timeBase);
                ret = av_seek_frame(_fmtCtx, streamIdx, ts, AVSEEK_FLAG_BACKWARD);
                if (ret < 0) { // seek失败
                    qDebug() << "seek失败" << Time << ts << streamIdx;
                    _seekTime = -1;
                } else {

                    //将最近关键帧到seekTime里的视频包读入队列
                    while(1){
                        ret = av_read_frame(_fmtCtx, &keypkt);
                        if (ret == 0) {
                            if (keypkt.stream_index == _vStream->index) {
                                Time = av_q2d(_vStream->time_base) * keypkt.dts;
                                if(Time > _seekTime){
                                    addVideoPkt(keypkt);
                                    break;
                                }
                                addVideoPkt(keypkt);
                            }
                        }
                    }
                    _vSeekTime = _seekTime;
                    _seekTime = -1;
                    // 恢复时钟
                    _vTime = 0;

                }

            }
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
    freeFilter();
}

void VideoPlayer::fataError() {
    _state = Playing;
    stop();
    emit playFailed(this);
}

void VideoPlayer::updateSignal(){
    _previewMutex.lock();
    _previewMutex.signal();
    _previewMutex.unlock();
}

int VideoPlayer::initFilter(AVFilterGraph **graph, AVFilterContext **srcFilterCtx, AVFilterContext **sinkFilterCtx, char *value) {
    //注册过滤器
    //avfilter_register_all();
    *graph = avfilter_graph_alloc();

    //源过滤器和格式转换过滤器参数
    std::string s1="sample_rate="+std::to_string(_aDecodeCtx->sample_rate)+":sample_fmt="+av_get_sample_fmt_name(_aDecodeCtx->sample_fmt)+":channel_layout="+std::to_string(_aDecodeCtx->channel_layout);
    std::string s2="sample_rates="+std::to_string(_aDecodeCtx->sample_rate)+":sample_fmts="+av_get_sample_fmt_name(_aDecodeCtx->sample_fmt)+":channel_layouts="+std::to_string(_aDecodeCtx->channel_layout);

    //创建源过滤器
    const AVFilter *srcFilter=avfilter_get_by_name("abuffer");
    *srcFilterCtx=avfilter_graph_alloc_filter(*graph,srcFilter,"src");
    if (avfilter_init_str(*srcFilterCtx, s1.c_str()) < 0) {
        qDebug()<<"初始化源过滤器失败";
        return -1;
    }


    //创建变速过滤器
    const AVFilter *atempoFilter = avfilter_get_by_name("atempo");
    AVFilterContext *atempoFilterCtx = avfilter_graph_alloc_filter(*graph, atempoFilter, "atempo");
    AVDictionary *args = NULL;
    av_dict_set(&args, "tempo", value, 0);//根据value的值调节速度
    if (avfilter_init_dict(atempoFilterCtx, &args) < 0) {
        qDebug()<<"初始化变速过滤器失败";
        return -1;
    }


    //创建格式转化过滤器
    const AVFilter *aformatFilter = avfilter_get_by_name("aformat");
    AVFilterContext *aformatFilterCtx = avfilter_graph_alloc_filter(*graph, aformatFilter, "aformat");
    if (avfilter_init_str(aformatFilterCtx, s2.c_str()) < 0) {
        qDebug()<<"初始化格式转化过滤器失败";
        return -1;
    }


    //创建接收过滤器
    const AVFilter *sinkFilter=avfilter_get_by_name("abuffersink");
    *sinkFilterCtx=avfilter_graph_alloc_filter(*graph,sinkFilter,"sink");
    if (avfilter_init_dict(*sinkFilterCtx, NULL) < 0) {
        qDebug()<<"初始化变速过滤器失败";
        return -1;
    }

    //链接过滤器
    if(avfilter_link(*srcFilterCtx,0,atempoFilterCtx,0) != 0){
        qDebug()<<"没link成功变速过滤器";
        return -1;
    }
    if(avfilter_link(atempoFilterCtx,0,aformatFilterCtx,0) != 0){
        qDebug()<<"没link成功格式转化过滤器";
        return -1;
    }
    if(avfilter_link(aformatFilterCtx,0,*sinkFilterCtx,0) != 0){
        qDebug()<<"没link成功接收过滤器";
        return -1;
    }


    //配置图
    if (avfilter_graph_config(*graph, NULL) < 0) {
        qDebug()<<"配置graph失败";
        return -1;
    }

    return 0;
}

void VideoPlayer::freeFilter() {

    if(srcFilterCtx_1 != nullptr){
        avfilter_free(srcFilterCtx_1);
        srcFilterCtx_1 = nullptr;
    }
    if(sinkFilterCtx_1 != nullptr){
        avfilter_free(sinkFilterCtx_1);
        sinkFilterCtx_1 = nullptr;
    }
    if(graph_1 != nullptr){
        avfilter_graph_free(&graph_1);
        graph_1 = nullptr;
    }
    if(srcFilterCtx_2 != nullptr){
        avfilter_free(srcFilterCtx_2);
        srcFilterCtx_2 = nullptr;
    }
    if(sinkFilterCtx_2 != nullptr){
        avfilter_free(sinkFilterCtx_2);
        sinkFilterCtx_2 = nullptr;
    }
    if(graph_2 != nullptr){
        avfilter_graph_free(&graph_2);
        graph_2 = nullptr;
    }
    if(srcFilterCtx_3 != nullptr){
        avfilter_free(srcFilterCtx_3);
        srcFilterCtx_3 = nullptr;
    }
    if(sinkFilterCtx_3 != nullptr){
        avfilter_free(sinkFilterCtx_3);
        sinkFilterCtx_3 = nullptr;
    }
    if(graph_3 != nullptr){
        avfilter_graph_free(&graph_3);
        graph_3 = nullptr;
    }
    if(srcFilterCtx_4 != nullptr){
        avfilter_free(srcFilterCtx_4);
        srcFilterCtx_4 = nullptr;
    }
    if(sinkFilterCtx_4 != nullptr){
        avfilter_free(sinkFilterCtx_4);
        sinkFilterCtx_4 = nullptr;
    }
    if(graph_4 != nullptr){
        avfilter_graph_free(&graph_4);
        graph_4 = nullptr;
    }
}

void VideoPlayer::setSpeed(int index)
{
    currentSpeedIndex=index;
}

AVFormatContext* VideoPlayer::get_fmtCtx(){
    return _fmtCtx;
}

char* VideoPlayer::getfilename(){
    return _filename;
}
