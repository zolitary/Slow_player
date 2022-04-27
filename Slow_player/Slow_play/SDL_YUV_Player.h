#ifndef SDL_YUV_PLAYER_H
#define SDL_YUV_PLAYER_H

#include <iostream>
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
//自定义消息类型
#define REFRESH_EVENT   (SDL_USEREVENT + 1)     // 请求画面刷新事件
#define QUIT_EVENT      (SDL_USEREVENT + 2)     // 退出事件
//定义分辨率
#define YUV_WIDTH 320
#define YUV_HEIGHT 240
//定义YUV格式
#define YUV_FORMAT SDL_PIXELFORMAT_IYUV

using namespace std;


int refresh_video_timer(void *data);

int player();
#endif // SDL_YUV_PLAYER_H
