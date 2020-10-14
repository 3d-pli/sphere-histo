#include "renderdata.h"

RenderData* RenderData::_instance = NULL; // Needs forward initialization outside header.

/*! Singleton pattern getter for privately constructed instance
 * Checks wether an instance of RenderData exists, creates a new instance if not and returns it. */
RenderData * RenderData::getInstance()
{
    if(!_instance){
        _instance = new RenderData();
    }
    return _instance;
}


/*!
 * Retrieves number of points per triangle of the currently selected sphere depth
 * and the maximum number of points in one triangle of that depth.
 * Retrieves the rgb color values from the currently selected colormap
 * using one triangle's point count divided by the maximum point count.
 * For possible lighting extensions an alpha value can be given as optional parameter (defaults to 1)
*/ 
std::vector<float> RenderData::generateColorsForTriangles(float alpha /* = 1 */)  {

    SphereDepthData & currentSphere = spheres.at(currentSphereDepth);
    std::vector<std::list<QVector3D> > & pointsPerTriangle = currentSphere.pointsPerTriangle;


    std::vector<float> colorVector;         // Return vector
    colorVector.reserve(pointsPerTriangle.size() * 4 * 3);

    size_t maxOfPointsPerTriangle = currentSphere.getMaxOfPointsPerTriangle();
    unsigned int colorIndex;

    for(std::list<QVector3D>& trianglePoints : pointsPerTriangle){
        if(maxOfPointsPerTriangle == 0){
            // if no points exist, always use the 'lowest' color
            colorIndex = 0;
        } else {
            // (current triangles' points / max points per one triangle) * colorMaps' row count * colorMaps' column count
            colorIndex = int((trianglePoints.size() / double(maxOfPointsPerTriangle)) * 255) * 3;
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


std::vector<float> RenderData::generatePointsAsVector() const {
    std::vector<float> renderPoints;
    renderPoints.reserve(points.size()*3);

    for (auto point : points){
        renderPoints.push_back(point.x());
        renderPoints.push_back(point.y());
        renderPoints.push_back(point.z());
    }

    return renderPoints;
}

bool RenderData::getMirrorPoints() const
{
    return mirrorPoints;
}

void RenderData::setMirrorPointsAndRecalculate(bool value)
{
    mirrorPoints = value;
    loadPointsFromFile(last_filename);
}


std::vector<float> RenderData::getVerticesAtCurrentDepth() const {
    return spheres[currentSphereDepth].getVertices();
}

std::vector<float> RenderData::getVerticesAtDepth(unsigned short depth) const {
    if(depth <= 8){
        return spheres[depth].getVertices();
    } else {
        throw "Sphere depth needs to be between 0 and 8";
    }
}

void RenderData::setSphereDepth(unsigned short depth)
{
    // Enforces maximum of 8 subdivisions
    unsigned short valid_depth = depth;
    if(depth > 8){
        valid_depth = 8;
    }
    currentSphereDepth = valid_depth;
    return;
}


void RenderData::setColorMap(QString colorMapName){

    cm::ColorMapName colorMapEnumValue = cm::stringToColorEnum.at(colorMapName.toStdString());

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

/*!
 * Loads data points from given .npy-file containing a double precision Nx3-Numpy-array,
 * mirrors them and saves them into points list.
 * Triggers recalculation of sphere with newly loaded points.
 */
void RenderData::loadPointsFromFile(std::string filename){
    try{
        // Load npy array from file
        cnpy::NpyArray np_points = cnpy::npy_load(filename);

        // check number of rows and columns
        size_t row_size = np_points.shape[0];
        size_t column_size = np_points.shape[1];
        if(column_size != 3){
            throw "Invalid input file";
        }

        double * pointData = np_points.data<double>();

        // clear previous data from point list
        points.clear();
        if(!np_points.fortran_order){
            // iterate over rows and save both the respective point and its mirrored version into points list
            for(size_t i = 0; i < row_size * column_size; i += column_size){

                // TODO: make data type depend on word size of np_points (4 -> float, 8 -> double) - mabye use a template?
                QVector3D point = {
                                static_cast<float>(pointData[i]),
                                static_cast<float>(pointData[i+1]),
                                static_cast<float>(pointData[i+2])
                                            };
                points.push_back(point);

                if(this->mirrorPoints){
                    QVector3D mirroredPoint = {
                                    - static_cast<float>(pointData[i]),
                                    - static_cast<float>(pointData[i+1]),
                                    - static_cast<float>(pointData[i+2])
                                                };
                    points.push_back(mirroredPoint);
                }
            }
        }else{
            // iterate over rows and save both the respective point and its mirrored version into points list
            for(size_t i = 0; i < row_size; ++i){

                // TODO: make data type depend on word size of np_points (4 -> float, 8 -> double) - mabye use a template?
                QVector3D point = {
                                static_cast<float>(pointData[i]),
                                static_cast<float>(pointData[i+(1*row_size)]),
                                static_cast<float>(pointData[i+(2*row_size)])
                                            };
                points.push_back(point);

                if(this->mirrorPoints){
                    QVector3D mirroredPoint = {
                                    - static_cast<float>(pointData[i]),
                                    - static_cast<float>(pointData[i+(1*row_size)]),
                                    - static_cast<float>(pointData[i+(2*row_size)])
                                                };
                    points.push_back(mirroredPoint);
                }
            }
        }
        this->last_filename = filename;

    } catch(std::string str){
        std::cerr << str << std::endl;
    } catch(std::runtime_error e){
        std::cerr << e.what() << std::endl;
        points.clear();
        return;
    }

    // Recalculate spheres with new points
    spheres.clear();
    for(int i = 0; i <= 8; ++i){
        calculateNextSubdivision();
    }
}

bool RenderData::getIcosphereSelected() const
{
    return icosphereSelected;
}

void RenderData::setIcosphereSelected(bool value)
{
    icosphereSelected = value;
}

bool RenderData::getPointsSelected() const
{
    return pointsSelected;
}

void RenderData::setPointsSelected(bool value)
{
    pointsSelected = value;
}


RenderData::RenderData() :
    points(),
    spheres(),
    currentSphereDepth(-1),
    icosphereSelected(true),
    pointsSelected(true),
    mirrorPoints(true),
    last_filename(""),
    colorMap(cm::_viridis_data)
{
    for(int i = 0; i <= 8; ++i){
        calculateNextSubdivision();
    }
    setSphereDepth(3);
}


/*! Generates first 'sphere', a 20-sided icosahedron,
 * and its respective points per triangle and saves them as SphereDepthData in spheres
*/
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
    if(!(spheres.empty())){
        std::cerr << "Error while trying to generate icosahedron - there already exists a sphere at depth 0" << std::endl;
        return;
    }
    spheres.push_back(SphereDepthData(0, verticesForDepthZero, pointsPerTriangle));
}


/*!
 * Calculates vertices and points per triangle for next sphere in spheres based on data
 * from last calculated sphere. If no sphere exists, triggers 'generateIcosahedronAtDepthZero'
 * instead.
*/
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

    for(std::list<QVector3D>::iterator i = pointList.begin(); i != pointList.end(); ){
       glm::vec3 point = {i->x(), i->y(), i->z()};
       if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
          pointsInTriangle.push_back(*i);
          i = pointList.erase(i);
       } else {
          ++i;
       }
    }
    return pointsInTriangle;
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


void RenderData::insertTriangleIntoVerticesVector(std::vector<float>& vertVec, float * vec1, float * vec2, float * vec3){
    float v1[3] = {vec1[0], vec1[1], vec1[2]};
    float v2[3] = {vec2[0], vec2[1], vec2[2]};
    float v3[3] = {vec3[0], vec3[1], vec3[2]};
    vertVec.insert(vertVec.end(), std::begin(v1), std::end(v1));
    vertVec.insert(vertVec.end(), std::begin(v2), std::end(v2));
    vertVec.insert(vertVec.end(), std::begin(v3), std::end(v3));
}


bool RenderData::pointInFirstQuadrantAfterTransformation(const glm::vec3 &point, const glm::mat3 &transformationMatrix){

    glm::vec3 transformedPoint = transformationMatrix * point;             // point in coordinate system of triangle vertices
//    if(this->spheres.size() == 1){
//        std::cout << "POINT: " << point[0] << "   " << point[1] << "   " << point[2] << "   " << std::endl;
//        std::cout << "NORMALIZED POINT: " << normalizedPoint[0] << "   " << normalizedPoint[1] << "   " << normalizedPoint[2] << "   " << std::endl;
//        std::cout << "TRANSFORMATION MATRIX: " << transformationMatrix[0][0] << "   "
//                  << transformationMatrix[0][1] << "   " << transformationMatrix[0][2] << "   "
//                  << transformationMatrix[1][0] << "   " << transformationMatrix[1][1] << "   "
//                  << transformationMatrix[1][2] << "   " << transformationMatrix[2][0] << "   "
//                  << transformationMatrix[2][1] << "   " << transformationMatrix[2][2] << "   "
//                  << std::endl;
//        std::cout << "TRANSFORMED POINT: " << transformedPoint[0] << "   " << transformedPoint[1] << "   " << transformedPoint[2] << "   " << std::endl;
//        std::cout << std::endl;
//    }


    if(transformedPoint[0] >= 0 && transformedPoint[1] >= 0 && transformedPoint[2] >= 0){
        return true;
    }else{
        return false;
    }

}

