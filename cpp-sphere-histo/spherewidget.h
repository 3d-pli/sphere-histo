#ifndef SPHEREWIDGET_H
#define SPHEREWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>

#include <GL/glu.h>
#include <cnpy.h>

#include <vector>
#define _USE_MATH_DEFINES   // for compatibility with older systems
#include <math.h>



//#include <../external-libraries/glm-master/glm/gtc/type_ptr.hpp>
//#include <../external-libraries/glm-master/glm/mat4x4.hpp>
//#include <../external-libraries/glm-master/glm/matrix.hpp>
//#include <../external-libraries/glm-master/glm/vec3.hpp>

#include "icosphere.h"
#include "resources/colormaps_matplotlib.h"
//#define SPHERE_DEPTH 4

#include <iostream>


class SphereWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SphereWidget(QWidget *parent = 0);
    ~SphereWidget(){}

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
//    float scaleFactor;
    float angle_x;
    float angle_y;
    int sphere_depth;
    float m_fovy;
    float aspectRatioWidthToHeight;
//    glm::mat4 m_projection;
    Icosphere ico;
};



#endif // SPHEREWIDGET_H
