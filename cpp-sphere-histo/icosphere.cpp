//#include "icosphere.h"
//// To avoid circular dependencies
//#include "renderdata.h"

//Icosphere::Icosphere()
//    /*:colorMap(std::begin(cm::_viridis_data), std::end(cm::_viridis_data))*/
//{
//   const float X  =.525731112119133606;
//   const float Z  =.850650808352039932;


//   vertices = {
//       {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
//       {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
//       {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
//   };

//   indices = {
//       {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
//       {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
//       {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
//       {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
//   };

////    colorLookupMap = {
////        {0, glm::vec3(0.27058823529411763, 0.4588235294117647, 0.7058823529411765)},
////        {1, glm::vec3(0.4549019607843137, 0.6784313725490196, 0.8196078431372549)},
////        {10, glm::vec3(0.6705882352941176, 0.8509803921568627, 0.9137254901960784)},
////        {100, glm::vec3(0.8784313725490196, 0.9529411764705882, 0.9725490196078431)},
////        {500, glm::vec3(1.0, 1.0, 0.7490196078431373)},
////        {1000, glm::vec3(0.996078431372549, 0.8784313725490196, 0.5647058823529412)},
////        {5000, glm::vec3(0.9921568627450981, 0.6823529411764706, 0.3803921568627451)},
////        {10000, glm::vec3(0.9568627450980393, 0.42745098039215684, 0.2627450980392157)},
////        {50000, glm::vec3(0.8431372549019608, 0.18823529411764706, 0.15294117647058825)},
////        {100000, glm::vec3(0,0,0)},
////    };
//   totalPoints = 0;

//}


////void Icosphere::drawIcosphere(unsigned int numberOfSubdivisions, std::list<QVector3D > pointsForHistogram){
////    totalPoints = pointsForHistogram.size();    // Muss gegen Max-Points ausgetauscht werden

////    for (std::vector<int> &i : indices){
////        float v1[3] = {vertices[i[0]][0], vertices[i[0]][1], vertices[i[0]][2]};
////        float v2[3] = {vertices[i[1]][0], vertices[i[1]][1], vertices[i[1]][2]};
////        float v3[3] = {vertices[i[2]][0], vertices[i[2]][1], vertices[i[2]][2]};

////        glm::mat3 transformationMatrix = {
////            v1[0], v1[1], v1[2],     // first COLUMN
////            v2[0], v2[1], v2[2],     // second COLUMN
////            v3[0], v3[1], v3[2]     // third COLUMN
////        };
////        transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
////        std::list<QVector3D> pointsForTriangle;

////        for(auto i = pointsForHistogram.begin(); i != pointsForHistogram.end(); ){
////            glm::vec3 point = {i->x(), i->y(), i->z()};
////            if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
////               //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
////               pointsForTriangle.push_back(*i);
////               i = pointsForHistogram.erase(i);
////            }else{
////               ++i;
////            }
////        }

////        subdivide(v1, v2, v3, numberOfSubdivisions, pointsForTriangle);
////    }
////}

//SphereDepthData Icosphere::getIcosahedron(){
//   std::vector<float> verticesForDepthZero;                     // Icosahedron consists of 20 triangles in 3D space, final size: 20 * 3 * 3
//   std::vector<std::list<QVector3D> > pointsPerTriangle;   // Points per each triangle, final size: 20

//   std::list<QVector3D> tmp_points;    // Temporary point list for current triangle

//   // Get all points from currently opened .npy file
//   std::list<QVector3D> pointList = RenderData::getInstance()->getPoints();

//   // Loop over 20 triangles of icosahedron
//   for(std::vector<int> &i : indices){

//       // Get the 3 triangle vertices
//       float v1[3] = {vertices[i[0]][0], vertices[i[0]][1], vertices[i[0]][2]};
//       float v2[3] = {vertices[i[1]][0], vertices[i[1]][1], vertices[i[1]][2]};
//       float v3[3] = {vertices[i[2]][0], vertices[i[2]][1], vertices[i[2]][2]};

//       // Turn triangle vertices to transformation matrix for point coverage evaluation
//       glm::mat3 transformationMatrix = getTransformationMatrix(v1, v2, v3);


//       // Filter out points of current triangle from all points
//       for(auto i = pointList.begin(); i != pointList.end(); ++i){
//           glm::vec3 point = {i->x(), i->y(), i->z()};
//           if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
//              tmp_points.push_back(*i);
//              i = pointList.erase(i);
//           }else{
//              ++i;
//           }
//       }

//       // Save data for current triangle and clear temporary list
//       verticesForDepthZero.insert(verticesForDepthZero.end(), std::begin(v1), std::end(v1));
//       verticesForDepthZero.insert(verticesForDepthZero.end(), std::begin(v2), std::end(v2));
//       verticesForDepthZero.insert(verticesForDepthZero.end(), std::begin(v3), std::end(v3));

//       pointsPerTriangle.push_back(tmp_points);

//       tmp_points.clear();

//   }
//   // Save data for current depth into respective SphereDepthData of the return vector
//   return SphereDepthData(0, verticesForDepthZero, pointsPerTriangle);
//}

//std::vector<SphereDepthData> Icosphere::calculateDepthData(short maxCalculatedDepth, unsigned short sphereDepth)
//{
//   std::vector<SphereDepthData> verticesPerDepth;

//   // To first calculate subdivision depth 0: Calculate undivided icosahedron triangles based on predefined vertices and indices and calculate respective points
////    if(maxCalculatedDepth == -1){        // TODO: Extract function

////        std::vector<float> verticesForCurrentDepth;                     // Icosahedron consists of 20 triangles in 3D space, final size: 20 * 3 * 3
////        std::vector<std::list<QVector3D> > pointsPerTriangle;   // Points per each triangle, final size: 20

////        std::list<QVector3D> tmp_points;    // Temporary point list for current triangle

////        // Get all points from currently opened .npy file
////        std::list<QVector3D> pointList = RenderData::getInstance()->getPoints();

////        // Loop over 20 triangles of icosahedron
////        for(std::vector<int> &i : indices){

////            // Get the 3 triangle vertices
////            float v1[3] = {vertices[i[0]][0], vertices[i[0]][1], vertices[i[0]][2]};
////            float v2[3] = {vertices[i[1]][0], vertices[i[1]][1], vertices[i[1]][2]};
////            float v3[3] = {vertices[i[2]][0], vertices[i[2]][1], vertices[i[2]][2]};

////            // Turn triangle vertices to transformation matrix for point coverage evaluation
////            glm::mat3 transformationMatrix = getTransformationMatrix(v1, v2, v3);


////            // Filter out points of current triangle from all points
////            for(auto i = pointList.begin(); i != pointList.end(); ++i){
////                glm::vec3 point = {i->x(), i->y(), i->z()};
////                if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
////                   tmp_points.push_back(*i);
////                   i = pointList.erase(i);
////                }else{
////                   ++i;
////                }
////            }

////            // Save data for current triangle and clear temporary list
////            verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v1), std::end(v1));
////            verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v2), std::end(v2));
////            verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v3), std::end(v3));

////            pointsPerTriangle.push_back(tmp_points);

////            tmp_points.clear();

////        }
////        // Save data for current depth into respective SphereDepthData of the return vector
////        verticesPerDepth[0] = SphereDepthData(0, verticesForCurrentDepth, pointsPerTriangle);
////                                // TODO: Alles fuer depth=0 hierzu auslagern: SphereDepthData calculateIcosahedron();
////    }
//   SphereDepthData lastSphere = RenderData::getInstance()->getSphereDataAtDepth(maxCalculatedDepth);

//   SphereDepthData currentSphere;

//   for(int i = RenderData::getInstance()->getMaxCalculatedDepth() + 1; i <= sphereDepth; ++i){
//       currentSphere = calculateSubdivision(lastSphere);
//       verticesPerDepth.push_back(currentSphere);
//       lastSphere = currentSphere;
//   }

//   return verticesPerDepth;
//}

//SphereDepthData Icosphere::calculateSubdivision(SphereDepthData lastSphere){
//   std::vector<float> lastDepthsVertices = lastSphere.getVertices();
//   std::vector<std::list<QVector3D> > lastDepthsPointsPerTriangle = lastSphere.getPointsPerTriangle();

//   std::vector<float> verticesForCurrentDepth(20 * pow(4, lastSphere.getDepth()+1) * 3 * 3);                     // Each subdivision has 4 times more triangles than its predecessor
//   std::vector<std::list<QVector3D> > pointsPerTriangle(20 * pow(4, lastSphere.getDepth()+1));   // Points per each triangle

//   // iterate over all triangles of the last sphere
//   for(size_t i = 0; i < lastDepthsPointsPerTriangle.size(); ++i){
//       // Current triangle vertices:
//       float v1[3] = {lastDepthsVertices[i*3*3], lastDepthsVertices[(i*3*3) +1], lastDepthsVertices[(i*3*3) +2]};
//       float v2[3] = {lastDepthsVertices[(i*3*3) + 3], lastDepthsVertices[(i*3*3) + 4], lastDepthsVertices[(i*3*3) + 5]};
//       float v3[3] = {lastDepthsVertices[(i*3*3) + 6], lastDepthsVertices[(i*3*3) + 7], lastDepthsVertices[(i*3*3) + 8]};

//       std::list<QVector3D> pointList = lastDepthsPointsPerTriangle.at(i);
//       std::list<QVector3D> tmp_points;    // Temporary point list for current triangle

//       // Generate intermediate vertices:
//       GLfloat v12[3], v23[3], v31[3];
//       for (unsigned short i = 0; i < 3; i++) {
//          v12[i] = v1[i]+v2[i];
//          v23[i] = v2[i]+v3[i];
//          v31[i] = v3[i]+v1[i];
//       }
//       normalize(v12);
//       normalize(v23);
//       normalize(v31);

//       // Triangle 1 (v1, v12, v31)

//       // Filter points inside triangle into tmp_points
//       glm::mat3 transformationMatrix = getTransformationMatrix(v1, v12, v31);
//       for(auto i = pointList.begin(); i != pointList.end(); ++i){
//           glm::vec3 point = {i->x(), i->y(), i->z()};
//           if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
//              tmp_points.push_back(*i);
//              i = pointList.erase(i);
//           }else{
//              ++i;
//           }
//       }
//       // Save data and clear tmp_points:
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v1), std::end(v1));
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v12), std::end(v12));
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v31), std::end(v31));

//       pointsPerTriangle.push_back(tmp_points);
//       tmp_points.clear();


//       // Triangle 2 (v2, v23, v12)

//       // Filter points inside triangle into tmp_points
//       transformationMatrix = getTransformationMatrix(v2, v23, v12);
//       for(auto i = pointList.begin(); i != pointList.end(); ++i){
//           glm::vec3 point = {i->x(), i->y(), i->z()};
//           if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
//              tmp_points.push_back(*i);
//              i = pointList.erase(i);
//           }else{
//              ++i;
//           }
//       }
//       // Save data and clear tmp_points:
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v2), std::end(v2));
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v23), std::end(v23));
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v12), std::end(v12));

//       pointsPerTriangle.push_back(tmp_points);
//       tmp_points.clear();

//       // Triangle 3 (v3, v31, v23)

//       // Filter points inside triangle into tmp_points
//       transformationMatrix = getTransformationMatrix(v3, v31, v23);
//       for(auto i = pointList.begin(); i != pointList.end(); ++i){
//           glm::vec3 point = {i->x(), i->y(), i->z()};
//           if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
//              tmp_points.push_back(*i);
//              i = pointList.erase(i);
//           }else{
//              ++i;
//           }
//       }
//       // Save data and clear tmp_points:
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v3), std::end(v3));
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v31), std::end(v31));
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v23), std::end(v23));

//       pointsPerTriangle.push_back(tmp_points);
//       tmp_points.clear();


//       // Triangle 4 (v12, v23, v31)

//       // Filter points inside triangle into tmp_points
//       transformationMatrix = getTransformationMatrix(v12, v23, v31);
//       for(auto i = pointList.begin(); i != pointList.end(); ++i){
//           glm::vec3 point = {i->x(), i->y(), i->z()};
//           if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
//              tmp_points.push_back(*i);
//              i = pointList.erase(i);
//           }else{
//              ++i;
//           }
//       }
//       // Save data and clear tmp_points:
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v12), std::end(v12));
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v23), std::end(v23));
//       verticesForCurrentDepth.insert(verticesForCurrentDepth.end(), std::begin(v31), std::end(v31));

//       pointsPerTriangle.push_back(tmp_points);
//       tmp_points.clear();
//       }
//   return SphereDepthData(lastSphere.getDepth()+1, verticesForCurrentDepth, pointsPerTriangle);
//}

////void Icosphere::subdivide(float *v1, float *v2, float *v3, long depth, std::list<QVector3D> &allRemainingPoints)
////// ACHTUNG: GEMEINSAME LISTE DER PUNKTE FUER ALLE DREIECKE, AUS DER BEIM MALEN GELÖSCHT WIRD. Funktioniert nur, solange nicht parallel.
////{
////   if (depth == 0) {
////      triangleColor(allRemainingPoints.size());
////      ////triangleColor(allRemainingPoints.size());
////      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
////      drawTriangle(v1, v2, v3);


////      // Draw triangle outlines:
////      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
////      glColor4f(0.8784313725490196, 0.9529411764705882, 0.9725490196078431, 0.2);
////      float v1l[3] = {v1[0] * 1.001f, v1[1] * 1.001f, v1[2] * 1.001f};
////      float v2l[3] = {v2[0] * 1.001f, v2[1] * 1.001f, v2[2] * 1.001f};
////      float v3l[3] = {v3[0] * 1.001f, v3[1] * 1.001f, v3[2] * 1.001f};

////      drawTriangle(v1l, v2l, v3l);

////      return;
////   }

////   GLfloat v12[3], v23[3], v31[3];
////   GLint i;
////   std::list<QVector3D> pointsInTriangle;

////   for (i = 0; i < 3; i++) {
////      v12[i] = v1[i]+v2[i];
////      v23[i] = v2[i]+v3[i];
////      v31[i] = v3[i]+v1[i];
////   }
////   normalize(v12);
////   normalize(v23);
////   normalize(v31);

////// Hässliches Copy-Paste für die 4 verschiedenen Unterdreiecke:
//////// TODO: IN METHODE AUSLAGERN

////// Dreieck 1
////   glm::dmat3 transformationMatrix = {
////       v1[0], v1[1], v1[2],     // first COLUMN!
////       v12[0], v12[1], v12[2],     // second COLUMN!
////       v31[0], v31[1], v31[2]     // third COLUMN!
////   };
////   transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
////   std::list<QVector3D> pointsForTriangle;

////   for(auto i = allRemainingPoints.begin(); i != allRemainingPoints.end(); ){
////       glm::vec3 point = {i->x(), i->y(), i->z()};
////       if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
////          //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
////          pointsForTriangle.push_back(*i);
////          i = allRemainingPoints.erase(i);
////       }else{
////          ++i;
////       }
////   }
////   subdivide(v1, v12, v31, depth-1, pointsForTriangle);

////// Dreieck 2
////   transformationMatrix = {
////       v2[0], v2[1], v2[2],     // first COLUMN!
////       v23[0], v23[1], v23[2],     // second COLUMN!
////       v12[0], v12[1], v12[2]     // third COLUMN!
////   };
////   transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
////   pointsForTriangle.clear();

////   for(auto i = allRemainingPoints.begin(); i != allRemainingPoints.end(); ){
////       glm::vec3 point = {i->x(), i->y(), i->z()};
////       if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
////          //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
////          pointsForTriangle.push_back(*i);
////          i = allRemainingPoints.erase(i);
////       }else{
////          ++i;
////       }
////   }
////   subdivide(v2, v23, v12, depth-1, pointsForTriangle);

////// Dreieck 3
////  transformationMatrix = {
////      v3[0], v3[1], v3[2],     // first COLUMN!
////      v31[0], v31[1], v31[2],     // second COLUMN!
////      v23[0], v23[1], v23[2]     // third COLUMN!
////  };
////  transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
////  pointsForTriangle.clear();

////  for(auto i = allRemainingPoints.begin(); i != allRemainingPoints.end(); ){
////      glm::vec3 point = {i->x(), i->y(), i->z()};
////      if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
////         //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
////         pointsForTriangle.push_back(*i);
////         i = allRemainingPoints.erase(i);
////      }else{
////         ++i;
////      }
////  }
////   subdivide(v3, v31, v23, depth-1, pointsForTriangle);

////// Dreieck 4
////      transformationMatrix = {
////          v12[0], v12[1], v12[2],     // first COLUMN!
////          v23[0], v23[1], v23[2],     // second COLUMN!
////          v31[0], v31[1], v31[2]     // third COLUMN!
////      };
////      transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
////      pointsForTriangle.clear();

////      for(auto i = allRemainingPoints.begin(); i != allRemainingPoints.end(); ){
////          glm::vec3 point = {i->x(), i->y(), i->z()};
////          if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
////             //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
////             pointsForTriangle.push_back(*i);
////             i = allRemainingPoints.erase(i);
////          }else{
////             ++i;
////          }
////      }
////   subdivide(v12, v23, v31, depth-1, pointsForTriangle);
////}

//void Icosphere::triangleColor(unsigned long pointsInTriangle){

//   int colorCode = int((float(pointsInTriangle)/totalPoints)*256);

////    glColor4f(RenderData::getInstance()->getColorMap[colorCode*3],
////            RenderData::getInstance()->getColorMap[(colorCode*3)+1],
////            RenderData::getInstance()->getColorMap[(colorCode*3)+2], 1);


////    for(std::map<size_t, glm::vec3>::iterator it = colorLookupMap.begin(); it != colorLookupMap.end(); ++it){
////        if(std::next(it) == colorLookupMap.end() || (it->first <= pointsInTriangle && std::next(it)->first > pointsInTriangle)){
////            glColor4f(it->second[0], it->second[1], it->second[2], 0.5);
////            break;
////        }
////    }
//}

//bool Icosphere::pointInFirstQuadrantAfterTransformation(const glm::vec3 &point, const glm::mat3 &transformationMatrix){

//   glm::vec3 transformedPoint = transformationMatrix * point;             // point in coordinate system of triangle vertices

//   if(transformedPoint[0] >= 0 && transformedPoint[1] >= 0 && transformedPoint[2] >= 0){
//       return true;
//   }else{
//       return false;
//   }

//}

//glm::mat3 Icosphere::getTransformationMatrix(float * v1, float * v2, float * v3){
//   glm::mat3 transformationMatrix = {
//       v1[0], v1[1], v1[2],        // first COLUMN
//       v2[0], v2[1], v2[2],        // second COLUMN
//       v3[0], v3[1], v3[2]         // third COLUMN
//   };
//   transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
//   return transformationMatrix;
//}
