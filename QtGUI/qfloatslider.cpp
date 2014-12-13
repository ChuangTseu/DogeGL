#include "qfloatslider.h"

QFloatSlider::QFloatSlider(QWidget *parent) :
    QSlider(parent)
{
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyFloatSliderMoved(int)));
}

void QFloatSlider::notifyFloatSliderMoved(int value) {
    float floatValue = value / 10.f;

    emit floatSliderMoved(floatValue);
    emit floatSliderMovedString(QString::number(floatValue, 'f', 1));
}
