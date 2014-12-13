#ifndef QFLOATSLIDER_H
#define QFLOATSLIDER_H

#include <QSlider>
#include <QString>

class QFloatSlider : public QSlider
{
    Q_OBJECT
public:
    explicit QFloatSlider(QWidget *parent = 0);

signals:
    void floatSliderMoved(float value);
    void floatSliderMovedString(QString value);

public slots:
    void notifyFloatSliderMoved(int value);
};

#endif // QFLOATSLIDER_H
