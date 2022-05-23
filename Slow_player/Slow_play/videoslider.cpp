#include "videoslider.h"
#include "videoplayer.h"
#include <QMouseEvent>
#include <QStyle>

VideoSlider::VideoSlider(QWidget *parent) : QSlider(parent) {
    setMouseTracking(true);//监听鼠标轨迹
    start=clock();
    x=0;
}

void VideoSlider::mouseReleaseEvent(QMouseEvent *ev) {
    int value = QStyle::sliderValueFromPosition(minimum(),
                maximum(),
                ev->pos().x(),
                width());
    setValue(value);
    QSlider::mouseReleaseEvent(ev);
    emit clicked(this);
}

void VideoSlider::mouseMoveEvent(QMouseEvent *ev){
    if(clock()-start >300)
    {
        int temp=ev->pos().x();
        if((x - temp < 3 && x - temp > 0) || (temp - x < 3 && temp - x > 0 ))
        {
            int value = QStyle::sliderValueFromPosition(minimum(),
                        maximum(),
                        ev->pos().x(),
                        width());
//            _previewMutex.lock();
//            _previewMutex.signal();
            emit preview(value, ev->pos().x());
//            _previewMutex.unlock();
        }
        start=clock();
        x=ev->pos().x();

    }
    QSlider::mouseMoveEvent(ev);
}

void VideoSlider::leaveEvent(QEvent *ev){
    emit mouseleave();
}

void VideoSlider::changeValue(int n){
    int valuenow = value();
    setValue(valuenow+n);
    emit clicked(this);
}
