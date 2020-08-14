#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

// STL
#include <list>
#include <vector>
#include <map>
#include <math.h>

// Qt
#include <QVector3D>
#include <QString>

// External libraries
#include <cnpy.h>
#include <../external-libraries/glm-master/glm/mat3x3.hpp>
#include <../external-libraries/glm-master/glm/matrix.hpp>
#include <../external-libraries/glm-master/glm/vec3.hpp>

// Internal
#include "resources/colormaps_matplotlib.h"
#include "spheredepthdata.h"

/*!
 * Singleton to calculate and store data of the
 *  - points to render
 *  - icosphere in various depths
 *  - current rendering depth
 *  - currently used colormap
 *  - color of the icosphere triangles according to
 *      the respective point count
*/
class RenderData
{
private:
    static RenderData * _instance;
    std::list<QVector3D> points;
    std::vector<SphereDepthData> spheres; //!< contains data of vertices and their respective points for all calculated depths
    std::vector<std::vector<float> > sphereBorderVertices; //!< vertices per depth in order to draw triangle borders
    short currentSphereDepth;       //!< 0-10: depth currently selected by application
    const float * colorMap;

    RenderData();                                   // [X]
    RenderData(const RenderData&) = delete;         // [X]

    void generateIcosahedronAtDepthZero();          // [X]
    void calculateNextSubdivision();                // [X]

    glm::mat3 getTransformationMatrix(float *v1, float *v2, float *v3);
    bool pointInFirstQuadrantAfterTransformation(const glm::vec3 &point, const glm::mat3 &transformationMatrix);

    inline void normalize(float v[3]) {
       float d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
       if (d == 0.0) {
          std::cerr << "zero length vector" << std::endl;
          return;
       }
       v[0] /= d; v[1] /= d; v[2] /= d;
    }
    void insertTriangleIntoVerticesVector(std::vector<float> &vertVec, float *vec1, float *vec2, float *vec3);

    std::list<QVector3D> filterPointsForTriangle(std::list<QVector3D> &pointList, float * v1, float * v2, float * v3);


public:

    static RenderData * getInstance();              // [X]

    // GETTER
    short getCurrentSphereDepth() const;
    const float * getColorMap() const;
    std::vector<float> getPointsAsVector(void);                     //
    inline std::list<QVector3D> getPoints(){ return points; }        // return by value because of deletions in subdivision routine
    const std::vector<float> getTriangleVerticesAndColorsAtCurrentDepth();
    std::vector<float> getColorsForTriangles(float alpha = 1);      // []
    std::vector<float> getVerticesAtCurrentDepth();

    // SETTER
    void setSphereDepth(short depth);      // [X]
    void setColorMap(QString colorMapName);

    void loadPointsFromFile(std::string filename);      // [X]

    friend class Icosphere;



};
#endif // VERTEX_DATA_H
