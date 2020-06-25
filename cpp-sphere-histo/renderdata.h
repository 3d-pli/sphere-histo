#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

// STL
#include <list>
#include <vector>

// Qt
#include <QVector3D>
#include <QString>

// External libraries
#include <cnpy.h>

// Internal
#include "icosphere.h"
#include "resources/colormaps_matplotlib.h"
#include "spheredepthdata.h"

/*!
 * Singleton to store data of the
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
    Icosphere ico;
    std::list<QVector3D> points;
    std::vector<SphereDepthData> vertices; // vertices per depth
    std::vector<std::vector<float> > sphereBorderVertices; // vertices per depth in order to draw triangle borders
    unsigned short currentSphereDepth;
    const float * colorMap;

    RenderData();
    RenderData(const RenderData&) = delete;

public:

    static RenderData * getInstance();

    // GETTER
    unsigned short getSphereDepth() const;
    const float * getColorMap() const;
    std::vector<float> getPointsAsVector(void);                     //
    inline std::list<QVector3D> getPoints(){ return points; }        // return by value because of deletions in subdivision routine
    const std::vector<float> getTriangleVerticesAtCurrentDepth();
    SphereDepthData getSphereDepthData(const short& depth);

    // SETTER
    void setSphereDepth(unsigned short value);
    void setColorMap(QString colorMapName);

    void loadPointsFromFile(std::string filename);

    friend class Icosphere;



    short getMaxCalculatedDepth() const;
};
#endif // VERTEX_DATA_H
