#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoplayer.h"
#include "videoslider.h"
#include "videowidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlayerStateChanged(VideoPlayer *player);//播放器状态改变
    void onPlayerTimeChanged(VideoPlayer *player);//进度条
    void onPlayerInitFinished(VideoPlayer *player);
    void onPlayerPlayFailed(VideoPlayer *player);
    void onSliderClicked(VideoSlider *slider);//seek
    void onSliderMouseFoucs(int seektime, int x);
    void on_stopBtn_clicked();
    void on_openFileBtn_clicked();
    void on_timeSlider_valueChanged(int value);
    void on_volumnSlider_valueChanged(int value);
    void on_playBtn_clicked();
    void on_muteBtn_clicked();

private:
    Ui::MainWindow *ui;
    VideoPlayer *_player, *preview_player;
    VideoWidget *preview;
    QString getTimeText(int value);
};
#endif // MAINWINDOW_H
