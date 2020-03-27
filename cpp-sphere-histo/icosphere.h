#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include <vector>
#include <math.h>
#include <QOpenGLFunctions>

#include <../external-libraries/glm-master/glm/mat3x3.hpp>
#include <../external-libraries/glm-master/glm/matrix.hpp>
#include <../external-libraries/glm-master/glm/vec3.hpp>

#include <iostream>



class Icosphere : protected QOpenGLFunctions
{
public:
    Icosphere();

//    inline std::vector<std::vector<double> > getVertices() { return vertices; }
//    inline std::vector<std::vector<double> > getIndices() { return indices; }

    void drawIcosphere(unsigned int numberOfSubdivisions, std::vector<std::vector<double> > points);  // ACHTUNG NOCH CALL BY VALUE! NOTFALLS ÄNDERN!

private:
    std::vector<std::vector<double> > vertices;
    std::vector<std::vector<double> > indices;
    size_t pointsOnTriangleBoundary;

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

    void triangleColor(unsigned long pointsInTriangle);

    /**
     * @brief pointInTriangleRange
     * @param point  - a 3D point
     * @param triangleVertices - 3 Vertices on a 3D sphere with radius 1
     * @return bool - wether a point lies inside the
     */
    bool pointInTriangleRange(const GLdouble point[3], const GLdouble triangleVertices[3][3] );

};

#endif // ICOSPHERE_H
