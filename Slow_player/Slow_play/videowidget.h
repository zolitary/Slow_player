#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>

/*
 *  显示视频 渲染视频
 */

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);

signals:

};

#endif // VIDEOWIDGET_H
