#include "renderdata.h"
RenderData* RenderData::_instance = 0;


std::vector<float> RenderData::getPointsAsVector(void){
    std::vector<float> renderPoints;
    renderPoints.reserve(points.size()*3);
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
    spheres(),
    sphereBorderVertices(),
    currentSphereDepth(-1),
    colorMap(cm::_viridis_data)
{
    setSphereDepth(3);
}

const float * RenderData::getColorMap() const
{
    return colorMap;
}

const std::vector<float> RenderData::getTriangleVerticesAndColorsAtCurrentDepth()
{
    if(static_cast<int>(spheres.size()) - 1 < currentSphereDepth){
        setSphereDepth(currentSphereDepth);
    }   // MEH - eher rausnehmen und Fehlerbehandlung anstossen

    SphereDepthData & currentSphere = spheres[currentSphereDepth];

    std::vector<float> vertsAndColors;
    vertsAndColors.reserve(currentSphere.vertices.size() + currentSphere.pointsPerTriangle.size() * 4);

    vertsAndColors.insert(vertsAndColors.end(), currentSphere.vertices.begin(), currentSphere.vertices.end());

    std::vector<float> colors = getColorsForTriangles();
    vertsAndColors.insert(vertsAndColors.end(), colors.begin(), colors.end());

    return vertsAndColors;
}

std::vector<float> RenderData::getVerticesAtCurrentDepth(){
    return spheres[currentSphereDepth].getVertices();
}

/*!
 * Loads data points from given .npy-file containing a double precision Nx3-Numpy-array, passes them to Spherewidget in a render-friendly form and notifies
 * the Icosphere to re-calculate the respective colors
 */
void RenderData::loadPointsFromFile(std::string filename){
    try{
        // Load npy array from file
        cnpy::NpyArray np_points = cnpy::npy_load(filename);

        // check number of rows and columns
        size_t row_size = np_points.shape[0];
        size_t column_size = np_points.shape[1];
        if(column_size != 3){
            // TODO: Maybe throw a "real" exception
            throw "Invalid input file";
        }


        double * pointData = np_points.data<double>();

        // clear previous data from point list
        points.clear();

        // iterate over rows and save both the respective point and its mirrored version into points list
        for(size_t i = 0; i < row_size * column_size; i += column_size){

            // TODO: make data type depend on word size of np_points (4 -> float, 8 -> double) - mabye use a template?
            QVector3D point = {
                            static_cast<float>(pointData[i]),
                            static_cast<float>(pointData[i+1]),
                            static_cast<float>(pointData[i+2])
                                        };
            QVector3D mirroredPoint = {
                            - static_cast<float>(pointData[i]),
                            - static_cast<float>(pointData[i+1]),
                            - static_cast<float>(pointData[i+2])
                                        };

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
    spheres.clear();
    setSphereDepth(currentSphereDepth);
}


short RenderData::getCurrentSphereDepth() const
{
    return currentSphereDepth;
}

void RenderData::setSphereDepth(short depth)
{
    //
    if(depth > 10){
        depth = 10;
    }

    int maxCalculatedDepth = spheres.size() - 1;

    if(depth > maxCalculatedDepth){
        if(spheres.empty()){
            generateIcosahedronAtDepthZero();
            maxCalculatedDepth = 0;
        }
        for(int i = 0; i < depth - maxCalculatedDepth; ++i){
            calculateNextSubdivision();
        }
    }
    currentSphereDepth = depth;
    return;

}

void RenderData::generateIcosahedronAtDepthZero(){
    const float X  =.525731112119133606;
    const float Z  =.850650808352039932;

    std::vector<QVector3D> startVertices = {
        {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
        {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
        {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
    };
    std::vector<std::vector<int> > startIndices = {
        {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
        {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
        {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
        {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
    };

    std::vector<float> verticesForDepthZero;                    // Icosahedron consists of 20 triangles in 3D space, final size: 20 * 3 * 3
    std::vector<std::list<QVector3D> > pointsPerTriangle;       // Points per each triangle, final size: 20

    // Copy points from currently opened .npy file for deleting points during the process
    std::list<QVector3D> pointList (points);

    // Loop over 20 triangles of icosahedron
    for(std::vector<int> &i : startIndices){

        // Get the 3 triangle vertices
        float v1[3] = {startVertices[i[0]][0], startVertices[i[0]][1], startVertices[i[0]][2]};
        float v2[3] = {startVertices[i[1]][0], startVertices[i[1]][1], startVertices[i[1]][2]};
        float v3[3] = {startVertices[i[2]][0], startVertices[i[2]][1], startVertices[i[2]][2]};

        // Filter out points of current triangle from all points
        pointsPerTriangle.push_back(filterPointsForTriangle(pointList, v1, v2, v3));

        // Save vertex data for current triangle
        insertTriangleIntoVerticesVector(verticesForDepthZero, v1, v2, v3);
    }
    // Save data for current depth into respective SphereDepthData in the vertices array
    Q_ASSERT(spheres.empty());
    spheres.push_back(SphereDepthData(0, verticesForDepthZero, pointsPerTriangle));
}

void RenderData::calculateNextSubdivision(){
    if(spheres.empty()){
        generateIcosahedronAtDepthZero();
        return;
    }
    int depthToCalculate = spheres.size();      // Last depth + 1 == spheres.size() - 1 + 1
    spheres.push_back(SphereDepthData());

    SphereDepthData &lastSphere = spheres.at(depthToCalculate - 1);
    SphereDepthData &currentSphere = spheres.at(depthToCalculate);

    currentSphere.depth = depthToCalculate;
    currentSphere.vertices.reserve(20 * pow(4, depthToCalculate) * 3 * 3);                     // Each subdivision has 4 times more triangles than its predecessor
    currentSphere.pointsPerTriangle.reserve(20 * pow(4, depthToCalculate));              // Points per each triangle

    // iterate over all triangles of the last sphere
    for(size_t i = 0; i < lastSphere.pointsPerTriangle.size(); ++i){
        // Current triangle vertices
        float v1[3] = {lastSphere.vertices[i*3*3], lastSphere.vertices[(i*3*3) +1], lastSphere.vertices[(i*3*3) +2]};
        float v2[3] = {lastSphere.vertices[(i*3*3) + 3], lastSphere.vertices[(i*3*3) + 4], lastSphere.vertices[(i*3*3) + 5]};
        float v3[3] = {lastSphere.vertices[(i*3*3) + 6], lastSphere.vertices[(i*3*3) + 7], lastSphere.vertices[(i*3*3) + 8]};

        // Generate intermediate vertices
        float v12[3], v23[3], v31[3];
        for (unsigned short i = 0; i < 3; i++) {
           v12[i] = v1[i]+v2[i];
           v23[i] = v2[i]+v3[i];
           v31[i] = v3[i]+v1[i];
        }
        normalize(v12);
        normalize(v23);
        normalize(v31);

        // Containers for calculation:
        std::list<QVector3D> biggerTrianglesPointList(lastSphere.pointsPerTriangle.at(i));
        std::list<QVector3D> tmp_smallerTrianglesPointList;    // Temporary point list for current triangle

//        currentSphere.vertices.insert(currentSphere.vertices.end(), std::begin(v1), std::end(v1));
//        currentSphere.vertices.insert(currentSphere.vertices.end(), std::begin(v12), std::end(v12));
//        currentSphere.vertices.insert(currentSphere.vertices.end(), std::begin(v31), std::end(v31));

        // Triangle 1 (v1, v12, v31)
        insertTriangleIntoVerticesVector(currentSphere.vertices, v1, v12, v31);
        currentSphere.pointsPerTriangle.push_back(filterPointsForTriangle(biggerTrianglesPointList, v1, v12, v31));

        // Triangle 2 (v2, v23, v12)
        insertTriangleIntoVerticesVector(currentSphere.vertices, v2, v23, v12);
        currentSphere.pointsPerTriangle.push_back(filterPointsForTriangle(biggerTrianglesPointList, v2, v23, v12));

        // Triangle 3 (v3, v31, v23)
        insertTriangleIntoVerticesVector(currentSphere.vertices, v3, v31, v23);
        currentSphere.pointsPerTriangle.push_back(filterPointsForTriangle(biggerTrianglesPointList, v3, v31, v23));

        // Triangle 4 (v12, v23, v31)
        insertTriangleIntoVerticesVector(currentSphere.vertices, v12, v23, v31);
        currentSphere.pointsPerTriangle.push_back(filterPointsForTriangle(biggerTrianglesPointList, v12, v23, v31));
    }
}

std::list<QVector3D> RenderData::filterPointsForTriangle(std::list<QVector3D> &pointList, float *v1, float *v2, float *v3){
    std::list<QVector3D> pointsInTriangle;
    glm::mat3 transformationMatrix = getTransformationMatrix(v1, v2, v3);

    for(std::list<QVector3D>::iterator i = pointList.begin(); i != pointList.end(); ++i){
       glm::vec3 point = {i->x(), i->y(), i->z()};
       if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
          pointsInTriangle.push_back(*i);
          i = pointList.erase(i);
       }
    }
    return pointsInTriangle;
}

void RenderData::insertTriangleIntoVerticesVector(std::vector<float>& vertVec, float * vec1, float * vec2, float * vec3){
    float v1[3] = {vec1[0], vec1[1], vec1[2]};
    float v2[3] = {vec2[0], vec2[1], vec2[2]};
    float v3[3] = {vec3[0], vec3[1], vec3[2]};
    vertVec.insert(vertVec.end(), std::begin(v1), std::end(v1));
    vertVec.insert(vertVec.end(), std::begin(v2), std::end(v2));
    vertVec.insert(vertVec.end(), std::begin(v3), std::end(v3));
}

void RenderData::setColorMap(QString colorMapName){
     std::cout << colorMapName.toStdString() << std::endl;

    cm::ColorMapName colorMapEnumValue = cm::qStringToColorEnum.at(colorMapName.toStdString());

    switch (colorMapEnumValue) {
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

std::vector<float> RenderData::getColorsForTriangles(float alpha /* = 1 */){
    // TODO: Find out why only colors for Icosahedron are calculated

    SphereDepthData & currentSphere = spheres.at(currentSphereDepth);
    std::vector<std::list<QVector3D> > & pointsPerTriangle = currentSphere.pointsPerTriangle;
    std::vector<float> colorVector;
    colorVector.reserve(pointsPerTriangle.size() * 4);

    size_t maxOfPointsPerTriangle = currentSphere.getMaxPointsPerTriangle();
    int colorIndex;

    for(std::list<QVector3D>& trianglePoints : pointsPerTriangle){
        if(maxOfPointsPerTriangle == 0){
            colorIndex = 0;
        } else {
            colorIndex = int((trianglePoints.size() / double(maxOfPointsPerTriangle)) * 256) * 3;  // (current triangles' points / max points per one triangle) * colorMaps' row count * colorMaps' column count
        }
        for(short i = 0; i < 3; ++i){
            colorVector.push_back(colorMap[colorIndex]);
            colorVector.push_back(colorMap[colorIndex + 1]);
            colorVector.push_back(colorMap[colorIndex + 2]);
            colorVector.push_back(alpha);
        }

    }
    return colorVector;
}

bool RenderData::pointInFirstQuadrantAfterTransformation(const glm::vec3 &point, const glm::mat3 &transformationMatrix){

    glm::vec3 transformedPoint = transformationMatrix * point;             // point in coordinate system of triangle vertices

    if(transformedPoint[0] >= 0 && transformedPoint[1] >= 0 && transformedPoint[2] >= 0){
        return true;
    }else{
        return false;
    }

}

glm::mat3 RenderData::getTransformationMatrix(float * v1, float * v2, float * v3){
    glm::mat3 transformationMatrix = {
        v1[0], v1[1], v1[2],        // first COLUMN
        v2[0], v2[1], v2[2],        // second COLUMN
        v3[0], v3[1], v3[2]         // third COLUMN
    };
    transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
    return transformationMatrix;
}
