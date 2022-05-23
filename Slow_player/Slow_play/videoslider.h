#ifndef VIDEOSLIDER_H
#define VIDEOSLIDER_H

#include <QSlider>
#include <time.h>
#include "condmutex.h"

class VideoSlider : public QSlider {
    Q_OBJECT
public:
    explicit VideoSlider(QWidget *parent = nullptr);
    void changeValue(int n);


signals:
    /** 点击事件 */
    void clicked(VideoSlider *slider);
    void preview(int seektime, int x);
    void mouseleave();


private:
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *ev) override;

    clock_t start;
    int x;
};

#endif // VIDEOSLIDER_H
