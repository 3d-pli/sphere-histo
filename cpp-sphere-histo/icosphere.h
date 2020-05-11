#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include "resources/colormaps_matplotlib.h"

#include <vector>
#include <list>
#include <map>

#include <math.h>
#include <QOpenGLFunctions>
#include <QVector3D>

#include <../external-libraries/glm-master/glm/mat3x3.hpp>
#include <../external-libraries/glm-master/glm/matrix.hpp>
#include <../external-libraries/glm-master/glm/vec3.hpp>

#include <iostream>



class Icosphere : protected QOpenGLFunctions
{
public:
    Icosphere();

//    inline std::vector<std::vector<float> > getVertices() { return vertices; }
//    inline std::vector<std::vector<float> > getIndices() { return indices; }

    void drawIcosphere(unsigned int numberOfSubdivisions, std::list<QVector3D > pointsForHistogram);
    // Call by value bei pointsForHistogram um Kopie der Liste zu erstellen, aus der Punkte nach Einbezug ins Histogramm gelöscht werden können
    void setColorMap(std::string colorMap);

private:
    std::vector<QVector3D> vertices;
    std::vector<std::vector<int> > indices;
    std::unordered_map<std::string, cm::colorMapName> qActionStringToEnum;
//    std::map<size_t, glm::vec3> colorLookupMap;     // Maps number of points in triangle to color as vec4
    size_t totalPoints;
    std::vector<float> colorMap;

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
