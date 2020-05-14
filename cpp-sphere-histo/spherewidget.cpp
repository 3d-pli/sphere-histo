#include "spherewidget.h"


SphereWidget::SphereWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      points(),
      last_x(0),
      last_y(0),
      angle_x(0),
      angle_y(0),
      sphere_depth(3),
      m_fovy(20),
      aspectRatioWidthToHeight(0),
      ico(Icosphere()),
      vbo_points(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
{

    vbo_points.create();
    loadPointsFromFile("../test_data/1.npy");
}

void SphereWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 1.0, 1.0);                         // background color
    glClearDepth(1.0);                                      // standard value 1.0
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // actually do the clearing
    glDepthFunc(GL_LESS);                                   // standard value GL_LESS (neue Fragmente werden angezeigt, wenn sie geringeren Tiefenwert haben)
//    glShadeModel(GL_SMOOTH);                                // setzt Farbverlauf zwischen verschiedenfarbigen Punkten (sonst: GL_FLAT)
    glEnable(GL_BLEND);

    glViewport(0, 0, QWidget::width(), QWidget::height());
    aspectRatioWidthToHeight = static_cast<float>(width()) / static_cast<float>(height());
    m_fovy = 20;
}
void SphereWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    aspectRatioWidthToHeight = float(w) / float(h);
}

void SphereWidget::paintGL() {
//    m_projection = glm::translate(glm::perspective(m_fovy, static_cast<float>(width()) / static_cast<float>(height()), 0.01, 10.0), glm::vec3(0,0,-2));
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fovy, aspectRatioWidthToHeight, 1., 20);
    gluLookAt(0.5, 0.5, 10 , 0 ,0 ,0.5 , 0, 1 ,0);

//    glLoadMatrixd(glm::value_ptr(m_projection));


    glMatrixMode(GL_MODELVIEW);

    // REACTION TO MOUSE INTERACTION
    glEnable(GL_DEPTH_TEST);
    glRotatef(angle_x, 0, 1, 0);
    glRotatef(angle_y, 1, 0, 0);
    angle_x = 0;
    angle_y = 0;

    glBegin(GL_POINTS);
    // GIVEN POINTS + MIRRORED POINTS
    glColor3f(0.2,0.2,0.2);
    vbo_points.bind();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawElements(GL_POINTS, vbo_points.size(), GL_FLOAT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    vbo_points.release();

//// Bisheriges Zeichnen der Punkte:
//    for(auto point : points){
//        glVertex3f(point[0], point[1], point[2]);
//    }
    glEnd();

    // Draw Icosahedron
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    ico.drawIcosphere(sphere_depth, points);
}

void SphereWidget::mousePressEvent(QMouseEvent * event){
    this->last_x = event->x();
    this->last_y = event->y();
}

void SphereWidget::mouseMoveEvent(QMouseEvent * event){
    std::cout << event->x() - this->last_x << ", " << event->y() - this->last_y << std::endl; //***
    float dx = event->x() - this->last_x;
    float dy = event->y() - this->last_y;

    this->angle_x = dx;
    this->angle_y = dy;

    this->last_x = event->x();
    this->last_y = event->y();
    this->update();
}

void SphereWidget::wheelEvent(QWheelEvent * event){
    float d = event->delta();
    m_fovy -= d/200.0;
    std::cout << m_fovy << std::endl;

    this->update();
}

void SphereWidget::setTriangleDepth(int depth){
    this->sphere_depth = depth;
    this->update();
}

void SphereWidget::openFile(std::string filename){
    loadPointsFromFile(filename);
    this->update();
}

void SphereWidget::setColorMap(std::string colorMap){
    this->ico.setColorMap(colorMap);
}

void SphereWidget::loadPointsFromFile(std::string filename){
    try{
        cnpy::NpyArray np_points = cnpy::npy_load(filename);
        size_t row_size = np_points.shape[0];
        size_t column_size = np_points.shape[1];
        assert(column_size == 3);       // TODO: Exception handling!

        GLfloat draw_points[row_size * column_size * 2];
        points.clear();

        for(size_t i = 0; i < row_size; ++i){
            QVector3D point = {
                            float(np_points.data<double>()[i]),
                            float(np_points.data<double>()[i+(1*row_size)]),
                            float(np_points.data<double>()[i+(2*row_size)])
                                        };
            QVector3D mirroredPoint = {
                            - float(np_points.data<double>()[i]),
                            - float(np_points.data<double>()[i+(1*row_size)]),
                            - float(np_points.data<double>()[i+(2*row_size)])
                                        };
            draw_points[i*6] = point.x();
            draw_points[i*6 + 1] = point.y();
            draw_points[i*6 + 2] = point.z();
            draw_points[i*6 + 3] = mirroredPoint.x();
            draw_points[i*6 + 4] = mirroredPoint.y();
            draw_points[i*6 + 5] = mirroredPoint.z();


            points.push_back(point);
            points.push_back(mirroredPoint);
        }
        vbo_points.bind();
        vbo_points.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        vbo_points.allocate(draw_points, row_size*column_size*2);;
        std::cout << vbo_points.size() << std::endl;
        vbo_points.release();

    } catch(std::runtime_error e){
        std::cerr << e.what() << std::endl;
        points.clear();
        return;
    }


}
