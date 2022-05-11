#include "videoplayer.h"
#include <thread>
#include <string.h>

#define ERROR_BUF \
    char errbuf[1024]; \
    av_strerror(ret,errbuf,sizeof(errbuf));

#define END(func) \
    if(ret<0){ \
        ERROR_BUF; \
        qDebug()<<#func<<"error"<<errbuf; \
        goto end; \
    }

#define RET(func) \
    if(ret<0){ \
        ERROR_BUF; \
        qDebug()<<#func<<"error"<<errbuf; \
        return ret; \
    }

VideoPlayer::VideoPlayer(QObject *parent)
    : QObject{parent}
{

}


VideoPlayer::~VideoPlayer(){

}

void VideoPlayer::play(){
    if(state_now==Playing) return;


    //读取文件线程
    std::thread([this](){
        readFile();
    }).detach();

    setState(Playing);
}

void VideoPlayer::pause(){
    if(state_now!=Playing) return;

    setState(Pause);
}

void VideoPlayer::stop(){
    if(state_now==Nosource) return;

    setState(Nosource);
}

bool VideoPlayer::isPlaying(){
    return state_now==Playing;
}

void VideoPlayer::setState(State state){
    if(state_now==state) return;



    state_now=state;
    emit stateChanged(this);
}

VideoPlayer::State VideoPlayer::getState(){
    return state_now;
}

void VideoPlayer::setFilename(const char* filename){
    //_filename=filename;
    strcpy(_filename,filename);
    //_filename="/Users/macbook/Desktop/极客时间/62-玩转Git三剑客（完结）/02丨安装Git.mp4";
    //qDebug()<<"!!!!!!!!"<<_filename;
}

void VideoPlayer::readFile(){

    int ret=0;
    //打开文件、创建解封装上下文
    //qDebug()<<"##########"<<_filename;
    ret=avformat_open_input(&_FmtCtx,_filename,nullptr,nullptr);
    END(avformat_open_input);

    //查看流信息
    ret=avformat_find_stream_info(_FmtCtx,nullptr);
    END(avformat_find_stream_info);

    //打印流信息
    av_dump_format(_FmtCtx,0,_filename,0);
    fflush(stderr);

    //初始化音频信息
    ret=initAudioInfo();
    if(ret<0){
        goto end;
    }
    //初始化视频信息
    ret=initVideoInfo();
    if(ret<0){
        goto end;
    }

    //读取数据
//    AVPacket pkt;
//    while(av_read_frame(_FmtCtx,&pkt)==0){
//        if(pkt.stream_index==_aStream->index){

//        }else if(pkt.stream_index==_vStream->index){

//        }
//    }
end:
   avcodec_free_context(&_aDecodeCtx);
   avcodec_free_context(&_vDecodeCtx);
   avformat_close_input(&_FmtCtx);
}

int VideoPlayer::initVideoInfo(){
    //初始化解码器
    int ret=initDecoder(&_vDecodeCtx,&_vStream,AVMEDIA_TYPE_VIDEO);
    RET(initDecoder);

    return 0;
}

int VideoPlayer::initAudioInfo(){
    //初始化解码器
    int ret=initDecoder(&_aDecodeCtx,&_aStream,AVMEDIA_TYPE_AUDIO);
    RET(initDecoder);

    return 0;
}

int VideoPlayer::initDecoder(AVCodecContext** decodeCtx,AVStream** stream,AVMediaType type){
    //根据type寻找合适的流信息 返回流索引

    int ret=av_find_best_stream(_FmtCtx,type,-1,-1,nullptr,0);
    RET(av_find_best_stream);

    //检验流
    int streamIdx=ret;
    *stream=_FmtCtx->streams[streamIdx];
    if(!stream){
        qDebug()<<"Fail to get stream";
        return -1;
    }

    //寻找合适的解码器
    const AVCodec *decoder=avcodec_find_decoder((*stream)->codecpar->codec_id);
    if(!decoder){
        qDebug()<<"Fail to find decoder"<<(*stream)->codecpar->codec_id;
        return -1;
    }

    //初始化解码上下文
    *decodeCtx=avcodec_alloc_context3(decoder);
    if(!decodeCtx){
        qDebug()<<"Fail to alloc decoder context";
        return -1;
    }
    //从流中拷贝参数到解码上下文中
    ret=avcodec_parameters_to_context(*decodeCtx,(*stream)->codecpar);
    RET(avcodec_parameters_to_context);
    //打开解码器
    ret=avcodec_open2(*decodeCtx,decoder,nullptr);
    RET(avcodec_open2);

    return 0;
}














