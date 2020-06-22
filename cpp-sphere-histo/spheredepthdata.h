#ifndef SPHEREDEPTHDATA_H
#define SPHEREDEPTHDATA_H

// STL
#include <vector>

// Qt
#include <QVector3D>

// internal
#include "resources/colormaps_matplotlib.h"

/*!
* Container class for all data regarding one sphere depth at a given set of points
*/
class SphereDepthData
{
public:
    SphereDepthData();      // probably unnecessary
    SphereDepthData(unsigned short depth, std::vector<float> vertices, std::vector<std::list<QVector3D> > pointsPerTriangle);
    ~SphereDepthData(){}

    // Getters
    std::vector<float> getVertices();

    // Setters
    void setVertices(std::vector<float> value);



    inline bool isEmpty(){  return vertices.empty(); }

    friend class icosphere;

private:
    unsigned short depth;      // probably unnecessary
    std::vector<float> vertices;
    std::vector<std::list<QVector3D> > pointsPerTriangle;
//    std::vector<float> colors;       // probably better to calculate color in render class in order to easily switch between colormaps
    size_t maxPointsPerTriangle;
};

#endif // SPHEREDEPTHDATA_H
