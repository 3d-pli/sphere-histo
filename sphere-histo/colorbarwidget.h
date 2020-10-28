#ifndef COLORMAPGRAPHICSVIEW_H
#define COLORMAPGRAPHICSVIEW_H


#include <QPainter>
#include <QLinearGradient>
#include <QColor>
#include <QRect>
#include <QWidget>
#include <QString>
#include <QPaintEvent>

#include "renderdata.h"

class ColorBarWidget : public QWidget
{
    Q_OBJECT

public:
    ColorBarWidget(QWidget *parent = nullptr);
    virtual ~ColorBarWidget(){}

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // COLORMAPGRAPHICSVIEW_H
