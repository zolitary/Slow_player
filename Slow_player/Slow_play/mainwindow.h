#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoplayer.h"
#include "videoslider.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getTimeText(int value);

private slots:
    void onPlayerStateChanged(VideoPlayer *player);
    void onPlayerInitFinished(VideoPlayer *player);
    void onPlayerPlayFailed(VideoPlayer *player);


    void on_openfileBtn_clicked();

    void on_currentSlider_valueChanged(int value);

    void on_volumnSlider_valueChanged(int value);

    void on_stopBtn_clicked();

    void on_playBtn_clicked();

private:
    Ui::MainWindow *ui;
    VideoPlayer *_player;
    const char* _readfilename;//保存主窗口获取的文件名

};
#endif // MAINWINDOW_H
