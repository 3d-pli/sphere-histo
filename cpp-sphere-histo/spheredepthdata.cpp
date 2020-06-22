#include "spheredepthdata.h"

SphereDepthData::SphereDepthData()
    : depth(0),
      vertices(),
      pointsPerTriangle(),
      maxPointsPerTriangle(0)
{
}

SphereDepthData::SphereDepthData(unsigned short depth, std::vector<float> vertices, std::vector<std::list<QVector3D> > pointsPerTriangle)
    : depth(depth),
      vertices(vertices),
      pointsPerTriangle(pointsPerTriangle)
{
}

std::vector<float> SphereDepthData::getVertices()
{
    return vertices;
}

void SphereDepthData::setVertices(std::vector<float> value)
{
    vertices = value;
}
