#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoplayer.h"
#include "videoslider.h"
#include "videowidget.h"
#include <qlistwidget.h>
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
    void onMouseLeaveSlider();

    void on_listBtn_clicked();
    void on_fileList_itemDoubleClicked(QListWidgetItem *item);


    void on_openDirBtn_clicked();
    void on_nextMediaBtn_clicked();
    void on_lastMediaBtn_clicked();
    void on_addFileBtn_clicked();
    void on_addFolderBtn_clicked();
    void on_clearListBtn_clicked();
    void on_back3sBtn_clicked();
    void on_speed3sBtn_clicked();

    void on_mutipleSpeed_currentIndexChanged(int index);



    void on_fillBtn_clicked();

private:
    Ui::MainWindow *ui;
    VideoPlayer *_player, *preview_player;
    VideoWidget *preview;
    QString getTimeText(int value);
    QSet<QString> file;//根据文件绝对路径判断是否重复
    QSet<QString>::iterator index;          //记录当前播放文件在播放列表中位置
};
#endif // MAINWINDOW_H
