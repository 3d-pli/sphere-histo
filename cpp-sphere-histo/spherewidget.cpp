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

}
