#ifndef SPHEREWIDGET_H
#define SPHEREWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QOpenGLBuffer>

#include <GL/glu.h>
#include <cnpy.h>

#include <vector>
#define _USE_MATH_DEFINES   // for compatibility with older systems
#include <math.h>


#include "icosphere.h"
#include "resources/colormaps_matplotlib.h"

#include <iostream>


class SphereWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SphereWidget(QWidget *parent = 0);
    inline ~SphereWidget(){}

    void setTriangleDepth(int depth);
    void openFile(std::string filename);
    void setColorMap(std::string mapName);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;

private:
    std::list<QVector3D> points;

    float last_x;
    float last_y;
    float angle_x;
    float angle_y;
    int sphere_depth;
    float m_fovy;
    float aspectRatioWidthToHeight;
//    glm::mat4 m_projection;
    Icosphere ico;
    void loadPointsFromFile(std::string filename);
    QOpenGLBuffer vbo_points;
};



#endif // SPHEREWIDGET_H
