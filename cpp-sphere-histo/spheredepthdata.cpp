#include "spheredepthdata.h"

SphereDepthData::SphereDepthData(short depth, std::vector<float> vertices, std::vector<std::list<QVector3D> > pointsPerTriangle)
    : depth(depth),
      vertices(vertices),
      pointsPerTriangle(pointsPerTriangle)
{
    updateMaxPointsPerTriangle();
}

std::vector<float> SphereDepthData::getVertices() const
{
    return vertices;
}

size_t SphereDepthData::getMaxPointsPerTriangle()
{
    updateMaxPointsPerTriangle();
    return maxPointsPerTriangle;
}

void SphereDepthData::setVertices(std::vector<float> value)
{
    vertices = value;
}

std::vector<std::list<QVector3D> > SphereDepthData::getPointsPerTriangle() const
{
    return pointsPerTriangle;
}

void SphereDepthData::setPointsPerTriangle(const std::vector<std::list<QVector3D> > &value)
{
    pointsPerTriangle = value;
    updateMaxPointsPerTriangle();
}

short SphereDepthData::getDepth() const
{
    return depth;
}

void SphereDepthData::updateMaxPointsPerTriangle(){
    maxPointsPerTriangle = 0;
    for(std::list<QVector3D>& points: pointsPerTriangle){
        if(points.size() > maxPointsPerTriangle){
            maxPointsPerTriangle = points.size();
        }
    }
}
