#include "renderdata.h"
RenderData* RenderData::_instance = 0;


std::vector<float> RenderData::getPointsAsVector(void){
    std::vector<float> renderPoints;
    for (auto point : points){
        renderPoints.push_back(point.x());
        renderPoints.push_back(point.y());
        renderPoints.push_back(point.z());
    }
    return renderPoints;
}

RenderData* RenderData::getInstance()
{
    if(!_instance){
        _instance = new RenderData();
    }
    return _instance;
}

short RenderData::getMaxCalculatedDepth() const
{
    return vertices.size() - 1;
}

RenderData::RenderData() :
    ico(),
    points(),
    vertices(),
    sphereBorderVertices(),
    currentSphereDepth(3),
    colorMap(cm::_viridis_data)
{
}

const float * RenderData::getColorMap() const
{
    return colorMap;
}

const std::vector<float> RenderData::getTriangleVerticesAtCurrentDepth()
{
    if(getMaxCalculatedDepth() < currentSphereDepth){
        std::vector<SphereDepthData> additionalDepths = ico.calculateDepthData(getMaxCalculatedDepth(), currentSphereDepth);
        vertices.insert(vertices.end(), additionalDepths.begin(), additionalDepths.end());
    }
    return vertices[currentSphereDepth].getVertices();
}

SphereDepthData RenderData::getSphereDepthData(const short &depth)
{
    if(vertices.empty()){
        vertices.push_back(ico.getIcosahedron());
    }
    if(depth == -1){
        return vertices.at(0);
    }
    if(vertices.size() < depth){
        std::vector<SphereDepthData> additionalDepths = ico.calculateDepthData(getMaxCalculatedDepth(), depth);
        vertices.insert(vertices.end(), additionalDepths.begin(), additionalDepths.end());
    }
    return vertices.at(depth);

}

/*
 * Loads data points from given .npy-file containing a double precision Nx3-Numpy-array, passes them to Spherewidget in a render-friendly form and notifies
 * the Icosphere to re-calculate the respective colors
 */
void RenderData::loadPointsFromFile(std::string filename){
    try{
        cnpy::NpyArray np_points = cnpy::npy_load(filename);
        size_t row_size = np_points.shape[0];
        size_t column_size = np_points.shape[1];
        if(column_size != 3){
            // TODO: Maybe throw a "real" exception
            throw "Invalid input file";
        }
        points.clear();
        std::vector<float> renderPoints(row_size*column_size*2);
        for(size_t i = 0; i < row_size; ++i){

            QVector3D point = {
                            float(np_points.data<double>()[i]),
                            float(np_points.data<double>()[i+(1*row_size)]),
                            float(np_points.data<double>()[i+(2*row_size)])
                                        };
            QVector3D mirroredPoint = {
                            - float(np_points.data<double>()[i]),
                            - float(np_points.data<double>()[i+(1*row_size)]),
                            - float(np_points.data<double>()[i+(2*row_size)])
                                        };
            renderPoints.push_back(point.x());
            renderPoints.push_back(point.y());
            renderPoints.push_back(point.z());
            renderPoints.push_back(mirroredPoint.x());
            renderPoints.push_back(mirroredPoint.y());
            renderPoints.push_back(mirroredPoint.z());

            points.push_back(point);
            points.push_back(mirroredPoint);
        }
    } catch(std::string str){
        std::cerr << str << std::endl;
    } catch(std::runtime_error e){
        std::cerr << e.what() << std::endl;
        points.clear();
        return;
    }
    vertices.clear();
    vertices = ico.calculateDepthData(getMaxCalculatedDepth(), currentSphereDepth);
}

unsigned short RenderData::getSphereDepth() const
{
    return currentSphereDepth;
}

void RenderData::setSphereDepth(unsigned short value)
{
    currentSphereDepth = value;
//    if(currentSphereDepth > maxCalculatedDepth){
//        std::vector<SphereDepthData> additionalDepths = ico.calculateDepthData(maxCalculatedDepth, currentSphereDepth);
//        vertices.insert(vertices.end(), additionalDepths.begin(), additionalDepths.end());
//        maxCalculatedDepth = currentSphereDepth;
//    }
//    // TODO: Icosphere rechnen lassen und maxcalcval ggfs anpassen!

}


void RenderData::setColorMap(QString colorMapName){
     std::cout << colorMapName.toStdString() << std::endl;

    cm::ColorMapName colorMapEnum = cm::qStringToColorEnum.at(colorMapName.toStdString());
    switch (colorMapEnum) {
    case cm::ColorMapName::Cividis:
        this->colorMap = cm::_cividis_data;
        break;
    case cm::ColorMapName::Inferno:
        this->colorMap = cm::_inferno_data;
        break;
    case cm::ColorMapName::Magma :
        this->colorMap = cm::_magma_data;
        break;
    case cm::ColorMapName::Plasma:
        this->colorMap = cm::_plasma_data;
        break;
    case cm::ColorMapName::Turbo:
        this->colorMap = cm::_turbo_data;
        break;
    case cm::ColorMapName::Viridis:
        this->colorMap = cm::_viridis_data;
        break;
    default:
        break;
    }
}
