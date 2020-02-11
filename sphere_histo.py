import sys

import numpy as np
from math import pow, e
import PySide2
from PySide2 import QtCore, QtWidgets, QtOpenGL, QtGui
from OpenGL.GL import *
from OpenGL.GLU import *
from random import random

points = np.load('test_data/1.npy')

print(points)
print(points.shape)


class GLWidget(QtWidgets.QOpenGLWidget):
    def __init__(self):
        super().__init__()
        self.last_x = 0
        self.last_y = 0
        self.scaleFactor = 0
        self.angle_x = 0
        self.angle_y = 0

    def initializeGL(self):
        glClearColor(1.0, 1.0, 1.0, 0)      # background color
        glClearDepth(1.0)                   # standard value 1.0
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)      # actually do the clearing
        glDepthFunc(GL_LESS)                # standard value GL_LESS (neue Fragmente werden angezeigt, wenn sie geringeren Tiefenwert haben)
        glShadeModel(GL_SMOOTH)             # setzt Farbverlauf zwischen verschiedenfarbigen Punkten (sonst: GL_FLAT)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(90.0, 1, 0, 10)
        glTranslatef(0, 0, -0.1)
        glScale(0.05, 0.05, 0.05)
        glColor3f(0, 0, 2.04)
        glMatrixMode(GL_MODELVIEW)

    def paintGL(self):
        glRotatef(self.angle_x, 0, 1, 0)
        glRotatef(self.angle_y, 1, 0, 0)
        if self.scaleFactor:
            glScale(self.scaleFactor, self.scaleFactor, self.scaleFactor)
            self.scaleFactor = 0

        glBegin(GL_POINTS)

        glColor3f(0.54, 0.90, 1.15)
        for v in points:
            glVertex3d(v[0], v[1], v[2])

        glColor3f(0, 0.53, 0.89)
        for v in points:
            glVertex3d(-v[0], -v[1], -v[2])

        glEnd()
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glColor3f(0, 0, 0.1)
        gluSphere(gluNewQuadric(), 1, 18, 18)
        glFlush()

    def resizeGL(self, width, height):
        QtWidgets.QOpenGLWidget.resizeGL(self, width, height)
        glViewport(0, 0, width, height)
        

    def mousePressEvent(self, event: QtGui.QMouseEvent):
        self.last_x = event.x()
        self.last_y = event.y()

    def mouseMoveEvent(self, event: QtGui.QMouseEvent):
        print(event.x() - self.last_x, event.y() - self.last_y)
        dx = event.x() - self.last_x
        dy = event.y() - self.last_y

        self.angle_x = dx
        self.angle_y = dy

        self.last_x = event.x()
        self.last_y = event.y()
        self.update()

    def wheelEvent(self, event: PySide2.QtGui.QWheelEvent):
        print(event.delta())
        self.scaleFactor = pow(e, event.delta() / 960)
        self.update()


if __name__ == "__main__":
    app = QtWidgets.QApplication([])

    widget = GLWidget()
    widget.resize(800, 800)

    widget.show()

    sys.exit(app.exec_())
