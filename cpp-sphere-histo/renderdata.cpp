#include "renderdata.h"
RenderData* RenderData::_instance = 0;


std::vector<float> RenderData::getRenderPoints(void){
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

RenderData::RenderData() :
    points(),
    vertices(),
    sphereDepth(3),
    colorMap(cm::_viridis_data)
{

}

const float * RenderData::getColorMap() const
{
    return colorMap;
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



//        vbo_points.setUsagePattern(QOpenGLBuffer::DynamicDraw);
////        Q_ASSERT(vbo_points.create());
//        Q_ASSERT(vbo_points.bind());
//        vbo_points.allocate(sizeof(draw_points));
//        vbo_points.write(0, draw_points, sizeof(draw_points));
//        vbo_points.release();
    } catch(std::string str){
        std::cerr << str << std::endl;
    } catch(std::runtime_error e){
        std::cerr << e.what() << std::endl;
        points.clear();
        return;
    }
}

unsigned short RenderData::getSphereDepth() const
{
    return sphereDepth;
}

void RenderData::setSphereDepth(unsigned short value)
{
    sphereDepth = value;
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
