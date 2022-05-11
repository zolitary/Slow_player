#include "videoslider.h"

#include <QMouseEvent>

VideoSlider::VideoSlider(QWidget *parent)
    : QSlider{parent}
{

}


void VideoSlider::mousePressEvent(QMouseEvent *ev){
    double ratio=ev->pos().x()*1.0/width();
    double len=maximum()-minimum();
    int value=len*ratio+minimum();
    setValue(value);

    QSlider::mousePressEvent(ev);
}
