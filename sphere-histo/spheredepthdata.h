#ifndef SPHEREDEPTHDATA_H
#define SPHEREDEPTHDATA_H

// STL
#include <vector>
#include <algorithm>

// Qt
#include <QVector3D>

// internal
#include "colormaps_matplotlib.h"

/*!
* Container class for all data regarding one sphere depth at a given set of points
*/
class SphereDepthData
{

public:

    // Constructors
    SphereDepthData()=default;
    SphereDepthData(short depth, std::vector<float> vertices, std::vector<std::list<QVector3D> > pointsPerTriangle);

    // Getters
    short getDepth() const;
    size_t getMaxOfPointsPerTriangle();
    std::vector<std::list<QVector3D> > getPointsPerTriangle() const;
    std::vector<float> getVertices() const;
    inline bool isEmpty() const {  return vertices.empty(); }

    // Setters
    void setVertices(std::vector<float> value);
    void setPointsPerTriangle(const std::vector<std::list<QVector3D> > &value);

    friend class RenderData;

private:

    short depth;      // probably unnecessary
    std::vector<float> vertices;
    std::vector<std::list<QVector3D> > pointsPerTriangle;
    size_t maxOfPointsPerTriangle;

    void updateMaxOfPointsPerTriangle();
};

#endif // SPHEREDEPTHDATA_H
