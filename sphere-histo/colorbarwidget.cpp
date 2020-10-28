#include "colorbarwidget.h"

ColorBarWidget::ColorBarWidget(QWidget *parent) :
    QWidget(parent)
{
}

void ColorBarWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient gradient(0, this->height(), 0, 0);

    const float * cm = RenderData::getInstance()->getColorMap();

    for (int i = 0; i < 256; ++i){
        gradient.setColorAt(i/255., QColor(cm[i*3]*255, cm[i*3+1]*255, cm[i*3+2]*255) );
    }
    painter.fillRect(QRect(0, 0, this->width(), this->height()), gradient);
    painter.drawRect(0, 0, this->width(), this->height());
}
