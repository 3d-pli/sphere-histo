#include "spherewidget.h"


SphereWidget::SphereWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      last_x(0),
      last_y(0),
//      scaleFactor(0),
      angle_x(0),
      angle_y(0),
      ico(Icosphere())
{

    // Load data from .npy file to 'points'
    cnpy::NpyArray np_points = cnpy::npy_load("../test_data/1.npy");
    size_t row_size = np_points.shape[0];
    size_t column_size = np_points.shape[1];
    assert(column_size == 3);       // TODO: Exception handling!

    for(unsigned i = 0; i < row_size; ++i){
        std::vector<double> point = {np_points.data<double>()[i],
                                     np_points.data<double>()[i+(1*row_size)],
                                     np_points.data<double>()[i+(2*row_size)]
                                    };
        std::vector<double> mirroredPoint = {
                                        - np_points.data<double>()[i],
                                        - np_points.data<double>()[i+(1*row_size)],
                                        - np_points.data<double>()[i+(2*row_size)]
                                    };
        points.push_back(point);
        points.push_back(mirroredPoint);
    }
    //    TEST ***
    //        int i = 0;
    //        for(auto point : points){
    //            std::cout << ++i << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")" << std::endl;
    //        }


    Icosphere ico;
}

void SphereWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 1.0, 0);                         // background color
    glClearDepth(1.0);                                      // standard value 1.0
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // actually do the clearing
    glDepthFunc(GL_LESS);                                   // standard value GL_LESS (neue Fragmente werden angezeigt, wenn sie geringeren Tiefenwert haben)
    glShadeModel(GL_SMOOTH);                                // setzt Farbverlauf zwischen verschiedenfarbigen Punkten (sonst: GL_FLAT)

    glViewport(0, 0, QWidget::width(), QWidget::height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float aspectRatioWidthToHeight = static_cast<float>(width()) / static_cast<float>(height());
    gluPerspective(20, aspectRatioWidthToHeight, 1.f, 20);
    gluLookAt(0.5, 0.5 ,10 , 0 ,0 ,0.5 , 0, 1 ,0);
    glMatrixMode(GL_MODELVIEW);

}
void SphereWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float aspectRatioWidthToHeight = static_cast<float>(width()) / static_cast<float>(height());
    gluPerspective(20, aspectRatioWidthToHeight, 1.f, 20);
    gluLookAt(0.5, 0.5 ,10 , 0 ,0 ,0.5 , 0, 1 ,0);
    glMatrixMode(GL_MODELVIEW);
}

void SphereWidget::paintGL() {
    // REACTION TO MOUSE INTERACTION
    glEnable(GL_DEPTH_TEST);
    glRotatef(angle_x, 0, 1, 0);
    glRotatef(angle_y, 1, 0, 0);
    angle_x = 0;
    angle_y = 0;
//    if (scaleFactor){
//        glScalef(scaleFactor, scaleFactor, scaleFactor);
//        scaleFactor = 0;
//    }

    glBegin(GL_POINTS);
    // GIVEN POINTS + MIRRORED POINTS
    glColor3f(0,0,0);
    for(auto point : points){
        glVertex3d(point[0], point[1], point[2]);
    }
    glEnd();

    // Draw Icosahedron
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    ico.drawIcosphere(SPHERE_DEPTH, points);
}

void SphereWidget::mousePressEvent(QMouseEvent * event){
    this->last_x = event->x();
    this->last_y = event->y();
}

void SphereWidget::mouseMoveEvent(QMouseEvent * event){
    std::cout << event->x() - this->last_x << ", " << event->y() - this->last_y << std::endl; //***
    double dx = event->x() - this->last_x;
    double dy = event->y() - this->last_y;

    this->angle_x = dx;
    this->angle_y = dy;

    this->last_x = event->x();
    this->last_y = event->y();
    this->update();
}

void SphereWidget::wheelEvent(QWheelEvent * event){
    //TODO: Fix scrolling and rotations

//    std::cout << (event->delta()) << std::endl; //***
//    this->scaleFactor = exp(event->delta() / 960.);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float aspectRatioWidthToHeight = static_cast<float>(width()) / static_cast<float>(height());
    gluPerspective(50* exp(event->delta() / 480.), aspectRatioWidthToHeight, 0.1f, 10);
    gluLookAt(0.5, 0.5 ,2 , 0 ,0 ,0.5 , 0, 1 ,0);

    glMatrixMode(GL_MODELVIEW);
    this->update();
}


