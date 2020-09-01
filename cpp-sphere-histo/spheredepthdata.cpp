#include "spheredepthdata.h"

SphereDepthData::SphereDepthData(short depth, std::vector<float> vertices, std::vector<std::list<QVector3D> > pointsPerTriangle)
    : depth(depth),
      vertices(vertices),
      pointsPerTriangle(pointsPerTriangle),
      maxOfPointsPerTriangle(0)
{
    updateMaxOfPointsPerTriangle();
}

std::vector<float> SphereDepthData::getVertices() const
{
    return vertices;
}

size_t SphereDepthData::getMaxOfPointsPerTriangle()
{
    updateMaxOfPointsPerTriangle();
    return maxOfPointsPerTriangle;
}

void SphereDepthData::setVertices(std::vector<float> value)
{
    vertices = value;
    updateMaxOfPointsPerTriangle();
}

std::vector<std::list<QVector3D> > SphereDepthData::getPointsPerTriangle() const
{
    return pointsPerTriangle;
}

void SphereDepthData::setPointsPerTriangle(const std::vector<std::list<QVector3D> > &value)
{
    pointsPerTriangle = value;
    updateMaxOfPointsPerTriangle();
}

short SphereDepthData::getDepth() const
{
    return depth;
}

void SphereDepthData::updateMaxOfPointsPerTriangle(){
    maxOfPointsPerTriangle = 0;
    for(std::list<QVector3D>& points: pointsPerTriangle){
        if(points.size() > maxOfPointsPerTriangle){
            maxOfPointsPerTriangle = points.size();
        }
    }
}
