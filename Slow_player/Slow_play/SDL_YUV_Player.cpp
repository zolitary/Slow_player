#include "SDL_YUV_Player.h"
extern int s_thread_exit_flag;
int refresh_video_timer(void *data)
{
    while(!s_thread_exit_flag){
        SDL_Event event;
        event.type=REFRESH_EVENT;
        SDL_PushEvent(&event);
        SDL_Delay(40);
    }
    s_thread_exit_flag=0;

    //push quit event
    SDL_Event event;
    event.type=QUIT_EVENT;
    SDL_PushEvent(&event);
    return 1;
}

int player(){
    //初始化SDL
    if(SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr,"Fail to initialize SDL - %s\n",SDL_GetError());
        return -1;
    }

    //SDL资源
    SDL_Event event;
    SDL_Rect rect;
    SDL_Window *window=nullptr;
    SDL_Renderer *renderer=nullptr;
    SDL_Texture *texture=nullptr;
    SDL_Thread *timer_thread=nullptr;//请求刷新线程
    uint32_t pixformat=YUV_FORMAT;//设置格式YUV420P

    //设置分辨率
    //YUV分辨率
    int video_width=YUV_WIDTH;
    int video_height=YUV_HEIGHT;
    //显示窗口分辨率
    int win_width=YUV_WIDTH;
    int win_height=YUV_HEIGHT;

    //YUV文件句柄
    FILE *video_fd=NULL;
    const char *yuv_path="/Users/macbook/Documents/GitHub/Slow_player/Slow_player/build-Slow_play-Desktop_x86_darwin_generic_mach_o_64bit-Debug/yuv420p_320x240.yuv";
    size_t video_buff_len=0;
    uint8_t *video_buf=NULL; //读取数据后先把放到buffer里面

    //YUV420P格式
    uint32_t y_frame_len=video_width*video_height;
    uint32_t u_frame_len=video_width*video_height/4;
    uint32_t v_frame_len=video_width*video_height/4;
    uint32_t yuv_frame_len=y_frame_len+u_frame_len+v_frame_len;

    //创建窗口
    window=SDL_CreateWindow("Simplest YUV Player",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             video_width, video_height,
                             SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if(!window){
        fprintf(stderr, "SDL: Fail to create Window, Error:%s\n",SDL_GetError());
        goto _FAIL;
    }

 //基于窗口创建渲染器
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr,"Failed to create renderer!\n");
        goto _FAIL;
    }
    // 基于渲染器创建纹理
    texture = SDL_CreateTexture(renderer,
                                pixformat,
                                SDL_TEXTUREACCESS_STREAMING,
                                video_width,
                                video_height);
    if(!texture){
        fprintf(stderr,"Failed to create texture!\n");
        goto _FAIL;
    }

    //分配空间
    video_buf=(uint8_t*)malloc(yuv_frame_len);
    if(!video_buf){
        fprintf(stderr,"Failed to alloce yuv frame space!\n");
        goto _FAIL;
    }

    //打开YUV文件
    video_fd=fopen(yuv_path,"rb");
    if( !video_fd ){
        fprintf(stderr, "Failed to open yuv file\n");
        goto _FAIL;
    }
    //创建请求刷新线程
    timer_thread=SDL_CreateThread(refresh_video_timer,NULL,NULL);
    while (1){
        // 收取SDL系统里面的事件
        SDL_WaitEvent(&event);

        if(event.type == REFRESH_EVENT) // 画面刷新事件
        {
            video_buff_len = fread(video_buf, 1, yuv_frame_len, video_fd);
            if(video_buff_len <= 0){
                fprintf(stderr, "Failed to read data from yuv file!\n");
                goto _FAIL;
            }
            // 设置纹理的数据 video_width = 320， plane
            SDL_UpdateTexture(texture, NULL, video_buf, video_width);

            // 显示区域，可以通过修改w和h进行缩放
            rect.x = 0;
            rect.y = 0;
            float w_ratio = win_width * 1.0 /video_width;
            float h_ratio = win_height * 1.0 /video_height;
            // 320x240
            rect.w = video_width * w_ratio;
            rect.h = video_height * h_ratio;
            //            rect.w = video_width * 0.5;
            //            rect.h = video_height * 0.5;

            // 清除当前显示
            SDL_RenderClear(renderer);
            // 将纹理的数据拷贝给渲染器
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            // 显示
            SDL_RenderPresent(renderer);
        }
        else if(event.type == SDL_WINDOWEVENT){
            //If Resize
            SDL_GetWindowSize(window, &win_width, &win_height);
            printf("SDL_WINDOWEVENT win_width:%d, win_height:%d\n",win_width,
                   win_height );
        }
        else if(event.type == SDL_QUIT){ //退出事件
            s_thread_exit_flag=1;
        }
        else if(event.type == QUIT_EVENT){
            break;
        }
    }




_FAIL:
    s_thread_exit_flag=1;      // 保证线程能够退出
    // 释放资源
    if(timer_thread)
        SDL_WaitThread(timer_thread, NULL); // 等待线程退出
    if(video_buf)
        free(video_buf);
    if(video_fd)
        fclose(video_fd);
    if(texture)
        SDL_DestroyTexture(texture);
    if(renderer)
        SDL_DestroyRenderer(renderer);
    if(window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
