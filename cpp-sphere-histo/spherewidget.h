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

#include <iostream>


class SphereWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SphereWidget(QWidget *parent = 0)
        : QOpenGLWidget(parent),
          points(std::vector<QVector3D>())
    {
        last_x = 0;
        last_y = 0;
        scaleFactor = 0;
        angle_x = 0;
        angle_y = 0;
        radius = 1;
        hAngle = M_PI / 180 * 72;
        vAngle = atan(1/2);
        cnpy::NpyArray np_points = cnpy::npy_load("../test_data/1.npy");
        size_t row_size = np_points.shape[0];
        size_t column_size = np_points.shape[1];
        assert(column_size == 3);
        for(unsigned i = 0; i < row_size; ++i){
            points.push_back(QVector3D(np_points.data<double>()[i],
                                       np_points.data<double>()[i+(1*row_size)],
                                       np_points.data<double>()[i+(2*row_size)]));
        }
//    TEST ***
//        int i = 0;
//        for(auto point : points){
//            std::cout << ++i << "(" << point.x() << ", " << point.y() << ", " << point.z() << ")" << std::endl;
//        }
    }

    ~SphereWidget(){}


protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;

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
