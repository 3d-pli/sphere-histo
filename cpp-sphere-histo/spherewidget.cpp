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
      vbo_sphereVertices(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
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
    std::cout << "LALALALA" << std::endl;
    updateSphereVertices();
    // TODO: hier sphereVertices das erste Mal fuellen!
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

    vbo_sphereVertices.bind();
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, vbo_sphereVertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    vbo_sphereVertices.release();

//// Bisheriges Zeichnen der Punkte:
//    for(auto point : points){
//        glVertex3f(point[0], point[1], point[2]);
//    }

    // render icosphere from icosahedron
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//    glBegin(GL_TRIANGLES);
////    if(!points.empty())
//        // TODO: Diesen Fehler in Icosphere abfangen
//            ico.drawIcosphere(renderData->getSphereDepth(), renderData->getPoints());
//    glEnd();
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
    std::vector<float> vertices = renderData->getTriangleVerticesAtCurrentDepth();
    Q_ASSERT(vbo_sphereVertices.isCreated());
    Q_ASSERT(vbo_sphereVertices.bind());
    vbo_sphereVertices.allocate(vertices.data(), vertices.size()*sizeof(float));
    vbo_sphereVertices.release();
    this->update();

}

void SphereWidget::updateTriangleColor()
{
    // TODO: Farben in VBO generieren abhaengig von aktueller Colormap und Punkten pro Triangle
    // - koennte als Funktionalitaet auch in der Icosphere liegen und hier nur updaten?
}

//        vbo_points.write(0, draw_points, sizeof(draw_points));
