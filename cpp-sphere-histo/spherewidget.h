#ifndef SPHEREWIDGET_H
#define SPHEREWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector3D>

#include <vector>
#define _USE_MATH_DEFINES   // for compatibility with older systems
#include <math.h>


class SphereWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SphereWidget(QWidget *parent = 0)
        : QOpenGLWidget(parent)
    {
        last_x = 0;
        last_y = 0;
        scaleFactor = 0;
        angle_x = 0;
        angle_y = 0;
        radius = 1;
        hAngle = M_PI / 180 * 72;
        vAngle = atan(1/2);
    }

    ~SphereWidget(){}


protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    std::vector<QVector3D> points;

    double last_x;
    double last_y;
    double scaleFactor;
    double angle_x;
    double angle_y;
    double radius;
    double hAngle;
    double vAngle;

};



#endif // SPHEREWIDGET_H
