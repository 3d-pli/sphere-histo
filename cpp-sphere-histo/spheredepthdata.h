#ifndef SPHEREDEPTHDATA_H
#define SPHEREDEPTHDATA_H

// STL
#include <vector>
#include <algorithm>

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
    SphereDepthData()=default;      // probably unnecessary
    SphereDepthData(unsigned short depth, std::vector<float> vertices, std::vector<std::list<QVector3D> > pointsPerTriangle);
//    ~SphereDepthData(){}

    // Getters
    std::vector<float> getVertices() const;
    size_t getMaxPointsPerTriangle();

    // Setters
    void setVertices(std::vector<float> value);



    inline bool isEmpty(){  return vertices.empty(); }

    friend class icosphere;

    std::vector<std::list<QVector3D> > getPointsPerTriangle() const;
    void setPointsPerTriangle(const std::vector<std::list<QVector3D> > &value);

    unsigned short getDepth() const;

private:
    unsigned short depth;      // probably unnecessary
    std::vector<float> vertices;
    std::vector<std::list<QVector3D> > pointsPerTriangle;
    size_t maxPointsPerTriangle;
//    std::vector<float> colors;       // probably better to calculate color in render class in order to easily switch between colormaps
};

#endif // SPHEREDEPTHDATA_H
