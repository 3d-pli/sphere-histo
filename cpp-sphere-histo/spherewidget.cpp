#include "spherewidget.h"


SphereWidget::SphereWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      renderData(RenderData::getInstance()),
      m_lastPosition(QPointF(0,0)),
      m_position(QVector3D(0, 0, 0)),
      m_rotation(QVector3D(0, 0, 0)),
      m_fovy(20),
      aspectRatioWidthToHeight(0),
      vbo_points(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer)),
      vbo_sphereVertices(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer)),
      vbo_vertexColors(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
{
    ;
}

void SphereWidget::initializeGL() {
    initializeOpenGLFunctions();

    aspectRatioWidthToHeight = static_cast<float>(width()) / static_cast<float>(height());

    vbo_points.create();
    vbo_points.release();
    Q_ASSERT(vbo_sphereVertices.create());
    vbo_sphereVertices.release();
    Q_ASSERT(vbo_sphereVertices.isCreated());
    vbo_vertexColors.create();
    vbo_vertexColors.release();

    updateSphereVertices();
    updateTriangleColor();
}
void SphereWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    aspectRatioWidthToHeight = float(w) / float(h);
}

void SphereWidget::paintGL() {
    // clear screen
    glClearColor(1.0, 1.0, 1.0, 1.0);                         // background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // actually do the clearing

    // update projection matrix
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fovy, aspectRatioWidthToHeight, 1., 20);
    gluLookAt(0.5, 0.5, 10 , 0 ,0 ,0.5 , 0, 1 ,0);

    // update view matrix (according to mouse interaction)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(m_position.x(), m_position.y(), 0);
    glRotatef(m_rotation.x(), 0.0f, 1.0f, 0.0f);
    glRotatef(m_rotation.y(), 1.0f, 0.0f, 0.0f);

    // set color and point size
    glColor3f(0.2,0.2,0.2);
    glPointSize(2);

    // bind point buffer
    vbo_points.bind();
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // enable states
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);

    // render points
    glDrawArrays(GL_POINTS, 0, vbo_points.size());

    // release buffer
    glDisableClientState(GL_VERTEX_ARRAY);
    vbo_points.release();

    // draw icosphere
    vbo_vertexColors.bind();
    glColorPointer(4, GL_FLOAT, 0, 0);

    vbo_sphereVertices.bind();
    glPointSize(1);
//    glColor4f(0.8,0.8,0.8, 0.6);
    glVertexPointer(3, GL_FLOAT, 0, 0);



    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, vbo_sphereVertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);

}

void SphereWidget::mousePressEvent(QMouseEvent * event){
    m_lastPosition = event->pos();
}

void SphereWidget::mouseMoveEvent(QMouseEvent * event){
//    std::cout << event->x() - m_lastPosition.x() << ", " << event->y() - m_lastPosition.y() << std::endl; //***
    float dx = event->x() - m_lastPosition.x();
    float dy = event->y() - m_lastPosition.y();

    m_lastPosition = event->pos();

    if (event->buttons() & Qt::LeftButton)
        {
            m_rotation.setX(m_rotation.x()+dx);
            m_rotation.setY(m_rotation.y()+dy);
        }
        else if(event->buttons() & Qt::RightButton)
        {
            m_position.setX(m_position.x() + dx/100.0);
            m_position.setY(m_position.y() - dy/100.0);
        }

    this->update();
}

void SphereWidget::wheelEvent(QWheelEvent * event){
    float d = event->delta();
    m_fovy -= d/200.0;
    this->update();
}


void SphereWidget::updatePoints(){
    // TODO: Test
    std::vector<float> points = renderData->getPointsAsVector();
    Q_ASSERT(vbo_points.bind());
    vbo_points.allocate(points.data(), points.size()*sizeof(float));
    vbo_points.release();
//    vbo_points.write(0, points.data(), points.size());
    this->update();
}

void SphereWidget::updateSphereVertices()
{
    // TODO: VBO mit aktuellen Vertices der Icosphere fuellen
    std::vector<float> vertices = renderData->getVerticesAtCurrentDepth();
    Q_ASSERT(vbo_sphereVertices.bind());
    vbo_sphereVertices.allocate(vertices.data(), vertices.size()*sizeof(float));
    vbo_sphereVertices.release();
    this->updateTriangleColor();
    this->update();

}

void SphereWidget::updateTriangleColor()
{
    std::vector<float> colors4f = renderData->getColorsForTriangles();
    Q_ASSERT(vbo_vertexColors.bind());
    vbo_vertexColors.allocate(colors4f.data(), colors4f.size() * sizeof(float));
    vbo_vertexColors.release();

    this->update();

    // TODO: Farben in VBO generieren abhaengig von aktueller Colormap und Punkten pro Triangle
    // - koennte als Funktionalitaet auch in der Icosphere liegen und hier nur updaten?
}
