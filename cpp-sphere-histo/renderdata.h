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

// Internal classes
#include "resources/colormaps_matplotlib.h"


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
public:
    inline std::vector<float> getRenderPoints(void);
    inline std::list<QVector3D> getPoints(){ return points; }        // return by value because of deletions in subdivision routine

    static RenderData * getInstance();


    void loadPointsFromFile(std::string filename);


    unsigned short getSphereDepth() const;
    void setSphereDepth(unsigned short value);

    const float * getColorMap() const;
    void setColorMap(QString colorMapName);


private:
    static RenderData * _instance;

    RenderData();
    RenderData(const RenderData&) = delete;
    std::list<QVector3D> points;
    std::vector<std::vector<float> > vertices; // vertices per depth
    unsigned short sphereDepth;
    const float * colorMap;


};
#endif // VERTEX_DATA_H
