#include "spherewidget.h"
#include <QMatrix4x4>


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
    if(!vbo_points.isCreated()){
        std::cerr << "Point VBO could not be created." << std::endl;
    }
    vbo_sphereVertices.create();
    vbo_sphereVertices.release();
    if(!vbo_sphereVertices.isCreated()){
        std::cerr << "Sphere vertex VBO could not be created." << std::endl;
    }
    vbo_vertexColors.create();
    vbo_vertexColors.release();
    if(!vbo_vertexColors.isCreated()){
        std::cerr << "Sphere color VBO could not be created." << std::endl;
    }
    if(QCoreApplication::arguments().size() > 1){
            renderData->loadPointsFromFile(QCoreApplication::arguments().at(1).toStdString());
    }
    updatePoints();
    updateSphereVertices();
    updateTriangleColor();


    glEnable(GL_DEPTH_TEST);
}

void SphereWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    aspectRatioWidthToHeight = float(w) / float(h);
}

void SphereWidget::paintGL() {
    // clear screen
    glClearColor(0.85,0.85,0.85, 1.0);                         // background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // actually do the clearing

    // update projection matrix
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float zNear = 1;
    float zFar = 20;
    float fH = tan( m_fovy / 360 * M_PI ) * zNear;
    float fW = fH * aspectRatioWidthToHeight;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
    glTranslatef(0,0,-10);


    // update view matrix (according to mouse interaction)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(m_position.x(), m_position.y(), 0);
    glRotatef(m_rotation.x(), 0.0f, 1.0f, 0.0f);
    glRotatef(m_rotation.y(), 1.0f, 0.0f, 0.0f);



    /// render points if selected
    if(renderData->getPointsSelected()){
        // set color and point size
        glColor3f(0.2,0.2,0.2);
        glPointSize(2);

        // bind point buffer
        vbo_points.bind();
        glVertexPointer(3, GL_FLOAT, 0, 0);

        // enable states
        glEnableClientState(GL_VERTEX_ARRAY);

        // render points
        glDrawArrays(GL_POINTS, 0, vbo_points.size());

        // release buffer
        glDisableClientState(GL_VERTEX_ARRAY);
        vbo_points.release();
        /// end: render points
    }

    /// render sphere
    // draw icosphere if selected
    if(renderData->getIcosphereSelected()){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPointSize(1);

        vbo_vertexColors.bind();
        glColorPointer(4, GL_FLOAT, 0, 0);

        vbo_sphereVertices.bind();
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glDrawArrays(GL_TRIANGLES, 0, vbo_sphereVertices.size());

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        vbo_vertexColors.release();
        vbo_sphereVertices.release();
    }

    paintCoordinateSystem();
}


void SphereWidget::paintCoordinateSystem(){
    // update projection matrix for coordinate system
    glViewport(0, 0, width()/8, height()/8);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float zNear = 1;
    float zFar = 20;
    float fH = tan( 20.0 / 360 * M_PI ) * zNear;
    float fW = fH * aspectRatioWidthToHeight;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
    glTranslatef(0,0,-10);


    // update view matrix (according to mouse interaction) for coordinate system
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glTranslatef(m_position.x(), m_position.y(), 0);
    glRotatef(m_rotation.x(), 0.0f, 1.0f, 0.0f);
    glRotatef(m_rotation.y(), 1.0f, 0.0f, 0.0f);


    // render coordinate system
    glPointSize(2);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1.5,0,0);

    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,1.5,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1.5);
    glEnd();
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
    std::vector<float> points = renderData->generatePointsAsVector();
    vbo_points.bind();
    vbo_points.allocate(points.data(), points.size()*sizeof(float));
    vbo_points.release();
//    vbo_points.write(0, points.data(), points.size());
    this->update();
}

void SphereWidget::updateSphereVertices()
{
    std::vector<float> vertices = renderData->getVerticesAtCurrentDepth();
    vbo_sphereVertices.bind();
    vbo_sphereVertices.allocate(vertices.data(), vertices.size()*sizeof(float));
    vbo_sphereVertices.release();
    this->updateTriangleColor();
    this->update();
}

void SphereWidget::updateTriangleColor()
{
    std::vector<float> colors4f = renderData->generateColorsForTriangles();
    vbo_vertexColors.bind();
    vbo_vertexColors.allocate(colors4f.data(), colors4f.size() * sizeof(float));
    vbo_vertexColors.release();

    this->update();

    // TODO: Farben in VBO generieren abhaengig von aktueller Colormap und Punkten pro Triangle
    // - koennte als Funktionalitaet auch in der Icosphere liegen und hier nur updaten?
}
