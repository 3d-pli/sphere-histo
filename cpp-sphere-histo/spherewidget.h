#ifndef SPHEREWIDGET_H
#define SPHEREWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMouseEvent>


#include <vector>
#define _USE_MATH_DEFINES   // for compatibility with older systems
#include <math.h>

#include <cnpy.h>

#include "icosphere.h"
#define SPHERE_DEPTH 5

#include <iostream>


class SphereWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SphereWidget(QWidget *parent = 0);

    ~SphereWidget(){}

//public slots:
//    void setIcosphereSubdivisions(unsigned numberOfSubdivisions);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;

private:
    std::vector<QVector3D> points;
//    std::vector<std::vector<double> > ico_vertices;
//    std::vector<std::vector<double> > ico_indices;
    double last_x;
    double last_y;
    double scaleFactor;
    double angle_x;
    double angle_y;
//    double radius;
//    const double H_ANGLE;
//    const double V_ANGLE;
    Icosphere ico;

};



#endif // SPHEREWIDGET_H
