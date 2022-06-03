QT       += core gui
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    condmutex.cpp \
    informationwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    videoplayer.cpp \
    videoplayer_audio.cpp \
    videoplayer_video.cpp \
    videoslider.cpp \
    videowidget.cpp

HEADERS += \
    ../../../../../Desktop/sl/audio-video-dev-tutorial-main 2/condmutex.h \
    condmutex.h \
    informationwindow.h \
    mainwindow.h \
    videoplayer.h \
    videoslider.h \
    videowidget.h

FORMS += \
    informationwindow.ui \
    mainwindow.ui

win32:{
    INCLUDEPATH += $$PWD/Dev/win/include
    LIBS += $$PWD/Dev/win/lib/avcodec.lib\
            $$PWD/Dev/win/lib/avdevice.lib\
            $$PWD/Dev/win/lib/avfilter.lib\
            $$PWD/Dev/win/lib/avformat.lib\
            $$PWD/Dev/win/lib/avutil.lib\
            $$PWD/Dev/win/lib/postproc.lib\
            $$PWD/Dev/win/lib/swresample.lib\
            $$PWD/Dev/win/lib/swscale.lib

    INCLUDEPATH += $$PWD/SDL2/SDL2_win/include
    LIBS += $$PWD/SDL2/SDL2_win/lib/x64/SDL2.lib
}

mac:{
    # 设置头文件路径
    INCLUDEPATH += $$PWD/Dev/macOS/include

    # 设置库文件路径
    LIBS += -L$$PWD/Dev/macOS/lib \
            -lavcodec \
            -lavdevice \
            -lavfilter \
            -lavformat \
            -lavutil \
            -lpostproc \
            -lswscale \
            -lswresample

    LIBS += $$PWD/SDL2/SDL2_mac/lib/libSDL2-2.0.0.dylib
    INCLUDEPATH += $$PWD/SDL2/SDL2_mac/include

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
        icon/icon.qrc
