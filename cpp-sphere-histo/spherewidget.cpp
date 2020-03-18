#include "spherewidget.h"


void SphereWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 1.0, 0);                         // background color
    glClearDepth(1.0);                                      // standard value 1.0
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // actually do the clearing
    glDepthFunc(GL_LESS);                                   // standard value GL_LESS (neue Fragmente werden angezeigt, wenn sie geringeren Tiefenwert haben)
    glShadeModel(GL_SMOOTH);                                // setzt Farbverlauf zwischen verschiedenfarbigen Punkten (sonst: GL_FLAT)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.02071,0.02071,-0.02071,0.02071,0.05,10);   // TEST!!!
    //gluPerspective(45.0, 1, 0, 10);
    glTranslated(0, 0, -0.1);
    glScalef(0.025, 0.025, 0.025);
    glColor3f(0, 0, 2.04);
    glMatrixMode(GL_MODELVIEW);

}
void SphereWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    glViewport(0, 0, w, h);
}

void SphereWidget::paintGL() {
        // REACTION TO MOUSE INTERACTION
        glRotatef(angle_x, 0, 1, 0);
        glRotatef(angle_y, 1, 0, 0);
        if (scaleFactor){
            glScalef(scaleFactor, scaleFactor, scaleFactor);
            scaleFactor = 0;
        }
        // TEST ***

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3d(0, 1, 0);
            glVertex3d(1, 0, 0);
            glVertex3d(0, 0, 1);

            glVertex3d(0, 1, 0);
            glVertex3d(0, 0, 1);
            glVertex3d(-1, 0, 0);

            glVertex3d(0, 1, 0);
            glVertex3d(-1, 0, 0);
            glVertex3d(0, 0, -1);

            glVertex3d(0, 1, 0);
            glVertex3d(0, 0, -1);
            glVertex3d(1, 0, 0);

        glEnd();


        glBegin(GL_POINTS);
        // GIVEN POINTS + MIRRORED POINTS
        glColor3f(0.54, 0.90, 1.15);
        for(auto point : points){
            glVertex3d(point.x(), point.y(), point.z());
        }

        // MIRRORED GIVEN POINTS
        glColor3f(0, 0.53, 0.89);
        for(auto point : points){
            glVertex3d(-point.x(), -point.y(), -point.z());
        }
        glEnd();
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
    std::cout << (event->delta()) << std::endl; //***
    this->scaleFactor = exp(event->delta() / 960.);
    this->update();
}
