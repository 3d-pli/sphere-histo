#ifndef SPHEREWIDGET_H
#define SPHEREWIDGET_H

// Qt
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QOpenGLBuffer>

// STL
#include <vector>
#define _USE_MATH_DEFINES   // for compatibility with older systems
#include <math.h>

// OpenGL
#include <GL/glu.h>

// Internal classes
#include "icosphere.h"
#include "resources/colormaps_matplotlib.h"
#include "renderdata.h"

// FOR DEBUGGING ONLY
#include <iostream>

/*!
 * A custom OpenGLWidget to render the scene from data collected inside the RenderData singleton.
*/
class SphereWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SphereWidget(QWidget *parent = 0);
    inline ~SphereWidget(){
        delete renderData;
    }

    void updatePoints();
    void updateSphereVertices();
    void updateTriangleColor();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;

private:
    RenderData * renderData;
//    std::list<QVector3D> points;

    QPointF m_lastPosition;
    QVector3D m_position;
    QVector3D m_rotation;

//    int sphere_depth;
    float m_fovy;
    float aspectRatioWidthToHeight;

    Icosphere ico;
    QOpenGLBuffer vbo_points;
    QOpenGLBuffer vbo_sphereVertices;

//    void loadPointsFromFile(std::string filename);
};



#endif // SPHEREWIDGET_H
