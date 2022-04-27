QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SDL_YUV_Player.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    SDL_YUV_Player.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += $$PWD/Dev/include
LIBS += $$PWD/Dev/lib/avcodec.lib\
        $$PWD/Dev/lib/avdevice.lib\
        $$PWD/Dev/lib/avfilter.lib\
        $$PWD/Dev/lib/avformat.lib\
        $$PWD/Dev/lib/avutil.lib\
        $$PWD/Dev/lib/postproc.lib\
        $$PWD/Dev/lib/swresample.lib\
        $$PWD/Dev/lib/swscale.lib

INCLUDEPATH += $$PWD/SDL2/include
LIBS += $$PWD/SDL2/lib/x64/SDL2.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
