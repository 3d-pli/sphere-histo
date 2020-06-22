#ifndef ICOSPHERE_H
#define ICOSPHERE_H

// STL
#include <vector>
#include <list>
#include <map>
#include <math.h>

// Qt
#include <QOpenGLFunctions>
#include <QVector3D>

// External libraries
#include <../external-libraries/glm-master/glm/mat3x3.hpp>
#include <../external-libraries/glm-master/glm/matrix.hpp>
#include <../external-libraries/glm-master/glm/vec3.hpp>

// Internal
#include "spheredepthdata.h"

// Debugging only
#include <iostream>



class Icosphere : protected QOpenGLFunctions
{
public:
    Icosphere();
    void calculateDepthData(unsigned short maxCalculatedDepth, unsigned short sphereDepth);

    // Obsolete
    void updateIcosphere(unsigned int numberOfSubdivisions, std::list<QVector3D > pointsForHistogram);  // Call by value bei pointsForHistogram um Kopie der Liste zu erstellen, aus der Punkte nach Einbezug ins Histogramm gelöscht werden können
    // Obsolete
    void drawIcosphere(unsigned int numberOfSubdivisions, std::list<QVector3D > pointsForHistogram);

private:
    std::vector<QVector3D> vertices;
    std::vector<std::vector<int> > indices;
//    std::unordered_map<std::string, cm::ColorMapName> qActionStringToEnum;
//    std::map<size_t, glm::vec3> colorLookupMap;     // Maps number of points in triangle to color as vec4
    size_t totalPoints;
//    std::vector<float> colorMap;

    void subdivide(float *v1, float *v2, float *v3, long depth, std::list<QVector3D> &allRemainingPoints);

    inline void drawTriangle(float * v1, float * v2, float * v3){
        glBegin(GL_TRIANGLES);
              glNormal3fv(v1); glVertex3fv(v1);
              glNormal3fv(v2); glVertex3fv(v2);
              glNormal3fv(v3); glVertex3fv(v3);
        glEnd();
    }

    inline void normalize(float v[3]) {
       GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
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
    bool pointInFirstQuadrantAfterTransformation(const glm::vec3 &point, const glm::mat3 &transformationMatrix);

};

#endif // ICOSPHERE_H
