#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

// STL
#include <list>
#include <vector>
#include <math.h>

// Qt
#include <QVector3D>
#include <QString>

// External libraries
#include <cnpy.h>
#include <../external-libraries/glm-master/glm/mat3x3.hpp>
#include <../external-libraries/glm-master/glm/matrix.hpp>
#include <../external-libraries/glm-master/glm/vec3.hpp>

// Internal dependencies
#include "resources/colormaps_matplotlib.h"
#include "spheredepthdata.h"

/*!
 * \class RenderData
 * \brief Singleton that stores and calculates all logical application data
 *
 *
 * Calculates and stores data of the
 *  - points currently loaded
 *  - icosphere in all calculated depths
 *  - current rendering depth
 *  - currently used colormap
 *  - color of the icosphere triangles according to
 *      the respective point count
*/
class RenderData
{
public:

    /*! \returns instance of singleton */
    static RenderData * getInstance();


    // GETTERS

    /*! \returns color values per point in vertices vector in format rgba
     *  \param alpha: (optional) alpha value for transparancy - not yet implemented
    */
    std::vector<float> getColorsForTriangles(float alpha = 1);

    /*! \returns all points from currently loaded .npy file in format [x1, y1, z1, x2, y2, z2, ... ] */
    std::vector<float> getPointsAsVector() const;

    /*! \returns all vertices at current depth in format [x1, y1, z1, x2, y2, z2, ... ] for use with OpenGL's GL_TRIANGLES */
    std::vector<float> getVerticesAtCurrentDepth() const;


    // SETTERS

    /*! Sets current sphere depth and if necessary calculates new sphere depth data.
     *  Maximum depth is 10 - all values above will be evaluated as 10.
     *  \param depth: sphere depth, representing the number of triangle subdivisions starting from an icosahedron at depth 0 */
    void setSphereDepth(short depth);

    /*! Sets currently selected colormap.
     *  \param colorMapName: Options: Cividis, Inferno, Magma, Plasma, Turbo, Viridis */
    void setColorMap(QString colorMapName);

    /*! Loads new set of points from .npy file with given filename. Deletes current points and respective calculations.
     *  \param filename: absolute or relative file path */
    void loadPointsFromFile(std::string filename);


private:

    static RenderData * _instance;
    std::list<QVector3D> points;                            //!< points currently loaded from .npy file
    std::vector<SphereDepthData> spheres;                   //!< data of the triangles' vertices and respective points for all depths calculated yet
    std::vector<std::vector<float> > sphereBorderVertices;  //!< vertices per depth in order to draw triangle borders
    short currentSphereDepth;                               //!< 0-10: depth currently selected by application (restriction enforced by setSphereDepth())
    const float * colorMap;

    RenderData();                                           //!< Sets initial sphere depth to 3
    RenderData(const RenderData&) = delete;                 //!< Explicit removal to guarantee strict singleton pattern


    /*! Calculates vertex data and points per respective triangles for depth 0 and saves it at spheres[0] */
    void generateIcosahedronAtDepthZero();

    /*! Calculates vertex data and points per respective triangles for next depth to be saved in spheres */
    void calculateNextSubdivision();


    // HELPER FUNCTIONS

    /*! \returns list of points lying 'inside' the triangle (v1, v2, v3) while eliminating these points from pointList.
     *  \param pointList: List of all points to search for points 'inside' triangle (v1, v2, v3)*/
    std::list<QVector3D> filterPointsForTriangle(std::list<QVector3D> & pointList, float * v1, float * v2, float * v3);

    /*! \returns the inverse of the 3x3 matrix consisting of the columns v1, v2 and v3.
     *  \param (v1, v2, v3) pointer to 3 float arrays containing 3 floats respectively */
    glm::mat3 getTransformationMatrix(float * v1, float * v2, float * v3);

    /*! inserts 3 3D float arrays (vec1, vec2 and vec3) into a vector vertVec */
    void insertTriangleIntoVerticesVector(std::vector<float> & vertVec, float * vec1, float * vec2, float * vec3);

    /*! normalizes a given 3D vector \param v */
    inline void normalize(float v[3]) {
       float d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
       if (d == 0.0) {
          std::cerr << "zero length vector" << std::endl;
          return;
       }
       v[0] /= d; v[1] /= d; v[2] /= d;
    }

    /*! \returns wether glm::vec3 point transformed by glm::mat3 transformationMatrix lies inside the first octant of 3D space.
     *  This represents wether point is counted as lying inside a triangle's space or not.*/
    bool pointInFirstQuadrantAfterTransformation(const glm::vec3 & point, const glm::mat3 & transformationMatrix);

};

#endif // VERTEX_DATA_H
