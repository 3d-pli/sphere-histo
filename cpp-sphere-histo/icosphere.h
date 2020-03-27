#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include <vector>
#include <math.h>
#include <QOpenGLFunctions>

#include <iostream>


class Icosphere : protected QOpenGLFunctions
{
public:
    Icosphere();

//    inline std::vector<std::vector<double> > getVertices() { return vertices; }
//    inline std::vector<std::vector<double> > getIndices() { return indices; }

    void drawIcosphere(unsigned int numberOfSubdivisions = 0);

private:
    std::vector<std::vector<double> > vertices;
    std::vector<std::vector<double> > indices;

    void subdivide(double *v1, double *v2, double *v3, long depth);

    inline void drawTriangle(double * v1, double * v2, double * v3){
        glBegin(GL_TRIANGLES);
              glNormal3dv(v1); glVertex3dv(v1);
              glNormal3dv(v2); glVertex3dv(v2);
              glNormal3dv(v3); glVertex3dv(v3);
        glEnd();
    }

    inline void normalize(double v[3]) {
       GLdouble d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
       if (d == 0.0) {
          std::cerr << "zero length vector" << std::endl;
          return;
       }
       v[0] /= d; v[1] /= d; v[2] /= d;
    }




};

#endif // ICOSPHERE_H
