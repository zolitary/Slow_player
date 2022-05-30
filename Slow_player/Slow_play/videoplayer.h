#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <list>
#include "condmutex.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/buffersink.h>
}

#define ERROR_BUF \
    char errbuf[1024]; \
    av_strerror(ret, errbuf, sizeof (errbuf));

#define CODE(func, code) \
    if (ret < 0) { \
        ERROR_BUF; \
        qDebug() << #func << "error" << errbuf; \
        code; \
    }

#define END(func) CODE(func, fataError(); return;)
#define RET(func) CODE(func, return ret;)
#define CONTINUE(func) CODE(func, continue;)
#define BREAK(func) CODE(func, break;)


class VideoPlayer : public QObject {
    Q_OBJECT
public:

    typedef enum {
        Stopped = 0,
        Playing,
        Paused
    } State;

    // 音量
    typedef enum {
        Min = 0,
        Max = 100
    } Volumn;

    //frame参数
    typedef struct {
        int width;
        int height;
        AVPixelFormat pixFmt;
        int size;
    } VideoSwsSpec;



    explicit VideoPlayer(QObject *parent = nullptr);
    ~VideoPlayer();

    void play();
    void play_preview();//创建预览线程
    void pause();
    void stop();
    void stopwithSignal();
    bool isPlaying();
    State getState();
    void setFilename(QString &filename);
    int getDuration();
    int getTime();//获取播放时间
    void setTime(int seekTime);//设置播放时间
    void setVolumn(int volumn);
    int getVolumn();
    void setMute(bool mute);//静音
    bool isMute();
    void updateSignal();
    void setSpeed(int index);//设置倍数播放

signals:
    void stateChanged(VideoPlayer *player);
    void timeChanged(VideoPlayer *player);
    void initFinished(VideoPlayer *player);
    void playFailed(VideoPlayer *player);
    void frameDecoded(VideoPlayer *player,uint8_t *data,VideoSwsSpec &spec);




private:
    //音频 参数
    typedef struct {
        int sampleRate;
        AVSampleFormat sampleFmt;
        int chLayout;
        int chs;
        int bytesPerSampleFrame;
    } AudioSwrSpec;

    AVCodecContext *_aDecodeCtx = nullptr;//解码器上下文
    AVStream *_aStream = nullptr;//音频流
    std::list<AVPacket> _aPktList;//音频包队列
    CondMutex _aMutex;//音频队列锁
    SwrContext *_aSwrCtx = nullptr;//音频重采样上下文
    AudioSwrSpec _aSwrInSpec, _aSwrOutSpec;//音频重采样输入、输出
    AVFrame *_aSwrInFrame = nullptr, *_aSwrOutFrame = nullptr;//重采样frame输入、输出
    int _aSwrOutIdx = 0;//音频重采样输出PCM的索引
    int _aSwrOutSize = 0;//音频重采样输出PCM的大小
    double _aTime = 0;//音频时钟，当前音频包对应的时间值
    bool _aCanFree = false;//音频资源是否可以释放
    int _aSeekTime = -1;//外面设置的当前播放时刻
    bool _hasAudio = false;//是否有音频流


    //音频函数
    int initAudioInfo();//初始化音频信息
    int initSwr();//初始化音频重采样
    int initSDL();//初始化SDL
    void addAudioPkt(AVPacket &pkt);//添加数据包到音频包列表中
    void clearAudioPktList();//清空音频包列表
    static void sdlAudioCallbackFunc(void *userdata, Uint8 *stream, int len);//SDL填充缓冲区的回调函数
    void sdlAudioCallback(Uint8 *stream, int len);//SDL填充缓冲区的回调函数
    int decodeAudio();//音频解码

    //视频参数
    AVCodecContext *_vDecodeCtx = nullptr;//解码上下文
    AVStream *_vStream = nullptr;//视频流
    AVFrame *_vSwsInFrame = nullptr, *_vSwsOutFrame = nullptr;//像素格式转换的输入、输出frame
    SwsContext *_vSwsCtx = nullptr;//像素格式转换的上下文
    VideoSwsSpec _vSwsOutSpec;//像素格式转换的输出frame的参数
    std::list<AVPacket> _vPktList;//视频包的列表
    CondMutex _vMutex;//视频包列表锁
    double _vTime = 0;//视频时钟，当前视频包对应的时间值
    bool _vCanFree = false;//视频资源是否可以释放
    int _vSeekTime = -1;//外面设置的当前播放时刻
    bool _hasVideo = false;//是否有视频流

    int initVideoInfo();//初始化视频信息
    int initSws();//初始化视频像素格式转换
    void addVideoPkt(AVPacket &pkt);//添加数据包到视频包列表中
    void clearVideoPktList();//清空视频包列表
    void decodeVideo();//解码视频


    //辅助
    CondMutex _previewMutex;
    AVFormatContext *_fmtCtx = nullptr;//解封装上下文
    bool _fmtCtxCanFree = false;//ftCtx是否可以释放
    int _volumn = Max;//音量
    bool _mute = false;//静音
    State _state = Stopped;//当前的状态
    char _filename[512];//文件名
    int _seekTime = -1;//外面设置的当前播放时刻

    int initDecoder(AVCodecContext **decodeCtx,AVStream **stream,AVMediaType type);//初始化解码器和解码上下文
    void setState(State state);//改变状态
    void readFile();//读取文件数据
    void startPreview();//用于预览的函数，仅有视频流
    void free();//释放资源
    void freeAudio();
    void freeVideo();
    void fataError();//错误处理


    //直接保存四个过滤器（0.5\1.0\1.5\2.0)
    int currentSpeedIndex=2;
    AVFilterGraph *graph_1 = nullptr;//过滤器链接图
    AVFilterContext *srcFilterCtx_1 = nullptr, *sinkFilterCtx_1 = nullptr;//过滤器源和接收过滤器上下文

    AVFilterGraph *graph_2 = nullptr;//过滤器链接图
    AVFilterContext *srcFilterCtx_2 = nullptr, *sinkFilterCtx_2 = nullptr;//过滤器源和接收过滤器上下文

    AVFilterGraph *graph_3 = nullptr;//过滤器链接图
    AVFilterContext *srcFilterCtx_3 = nullptr, *sinkFilterCtx_3 = nullptr;//过滤器源和接收过滤器上下文

    AVFilterGraph *graph_4 = nullptr;//过滤器链接图
    AVFilterContext *srcFilterCtx_4 = nullptr, *sinkFilterCtx_4 = nullptr;//过滤器源和接收过滤器上下文

    int initFilter(AVFilterGraph **graph, AVFilterContext **srcFilterCtx, AVFilterContext **sinkFilterCtx, char *value);//初始化过滤器
    void freeFilter();//释放过滤器





};

#endif // VIDEOPLAYER_H
