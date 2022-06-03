#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QWidget>

extern "C"{
#include <libavformat/avformat.h>
}

namespace Ui {
class informationWindow;
}

class informationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit informationWindow(QWidget *parent = nullptr);
    ~informationWindow();
    void updateinformation(AVFormatContext *update_fmtCtx,char *filename);

private:
    Ui::informationWindow *ui;
    AVFormatContext *_fmtCtx = nullptr;//解封装上下文
};

#endif // INFORMATIONWINDOW_H
