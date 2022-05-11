 #ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QDebug>
#include <string>
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libavformat/version.h>
    #include <libavutil/time.h>
    #include <libavutil/mathematics.h>
    #include <SDL.h>
    #ifdef __cplusplus
    #define __STDC_CONSTANT_MACROS
    #ifdef _STDINT_H
    #undef _STDINT_H
    #endif
    #include<stdint.h>
    #endif
}
#undef main

/*
 * 预处理视频数据 （解析文件）
 */
class VideoPlayer : public QObject
{
    Q_OBJECT
public:
    //播放器状态
    typedef enum{
        Nosource=0,//没有源
        Playing,
        Pause
    }State;

    explicit VideoPlayer(QObject *parent = nullptr);
    ~VideoPlayer();

    void play();
    void pause();
    void stop();
    bool isPlaying();
    State getState();
    void setFilename(const char* filename);

signals:
    void stateChanged(VideoPlayer *player);

private:
    State state_now=Nosource;
    char* _filename;
    void setState(State state);
    void readFile();

    //解封装
    AVFormatContext *_FmtCtx=nullptr;//解封装上下文
    AVCodecContext *_aDecodeCtx=nullptr,*_vDecodeCtx=nullptr;//音、视频解码上下文
    AVStream *_aStream=nullptr,*_vStream=nullptr;//音、视频流
    AVFrame *_aFrame=nullptr,*_vFrame=nullptr;//音、视频解码数据帧

    //初始化
    int initVideoInfo();//初始化视频信息
    int initAudioInfo();//初始化音频信息
    int initDecoder(AVCodecContext** decodeCtx,
                    AVStream** stream,
                    AVMediaType type);//初始化解码器、解码上下文


};

#endif // VIDEOPLAYER_H
