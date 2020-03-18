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
          last_x(0),
          last_y(0),
          scaleFactor(0),
          angle_x(0),
          angle_y(0),
          radius(1),
          H_ANGLE( M_PI / 180 * 72 ),       // 72 degree in rad
          V_ANGLE(atan(1./2))
    {   

    // Load data from .npy file to 'points'
        cnpy::NpyArray np_points = cnpy::npy_load("../test_data/1.npy");
        size_t row_size = np_points.shape[0];
        size_t column_size = np_points.shape[1];
        assert(column_size == 3);
        points = std::vector<QVector3D>(row_size);

        for(unsigned i = 0; i < row_size; ++i){

            points[i] = (QVector3D(np_points.data<double>()[i],
                                       np_points.data<double>()[i+(1*row_size)],
                                       np_points.data<double>()[i+(2*row_size)]));
        }
//    TEST ***
//        int i = 0;
//        for(auto point : points){
//            std::cout << ++i << "(" << point.x() << ", " << point.y() << ", " << point.z() << ")" << std::endl;
//        }

    // Calculate vertices for icosahedron
        ico_vertices = std::vector<std::vector<double> >(12);

        const double X  =.525731112119133606;
        const double Z  =.850650808352039932;
        ico_vertices = {
            {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
            {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
            {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
         };
        ico_indices = {
            {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
            {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
            {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
            {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };
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
    std::vector<std::vector<double> > ico_vertices;
    std::vector<std::vector<double> > ico_indices;
    double last_x;
    double last_y;
    double scaleFactor;
    double angle_x;
    double angle_y;
    double radius;
    const double H_ANGLE;
    const double V_ANGLE;
};



#endif // SPHEREWIDGET_H
