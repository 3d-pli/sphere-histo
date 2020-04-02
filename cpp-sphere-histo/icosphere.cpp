#include "icosphere.h"

Icosphere::Icosphere()
{
    const double X  =.525731112119133606;
    const double Z  =.850650808352039932;


    vertices = {
        {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
        {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
        {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
    };

    indices = {
        {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
        {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
        {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
        {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
    };

    colorLookupMap = {
        {0, glm::vec3(0.27058823529411763, 0.4588235294117647, 0.7058823529411765)},
        {1, glm::vec3(0.4549019607843137, 0.6784313725490196, 0.8196078431372549)},
        {10, glm::vec3(0.6705882352941176, 0.8509803921568627, 0.9137254901960784)},
        {100, glm::vec3(0.8784313725490196, 0.9529411764705882, 0.9725490196078431)},
        {500, glm::vec3(1.0, 1.0, 0.7490196078431373)},
        {1000, glm::vec3(0.996078431372549, 0.8784313725490196, 0.5647058823529412)},
        {5000, glm::vec3(0.9921568627450981, 0.6823529411764706, 0.3803921568627451)},
        {10000, glm::vec3(0.9568627450980393, 0.42745098039215684, 0.2627450980392157)},
        {50000, glm::vec3(0.8431372549019608, 0.18823529411764706, 0.15294117647058825)},
        {100000, glm::vec3(0,0,0)},
    };
}

void Icosphere::drawIcosphere(unsigned int numberOfSubdivisions, std::list<std::vector<double> > pointsForHistogram){

    for (std::vector<double> &i : indices){
        double v1[3] = {vertices[i[0]][0], vertices[i[0]][1], vertices[i[0]][2]};
        double v2[3] = {vertices[i[1]][0], vertices[i[1]][1], vertices[i[1]][2]};
        double v3[3] = {vertices[i[2]][0], vertices[i[2]][1], vertices[i[2]][2]};

        glm::dmat3 transformationMatrix = {
            v1[0], v1[1], v1[2],     // first COLUMN!
            v2[0], v2[1], v2[2],     // second COLUMN!
            v3[0], v3[1], v3[2]     // third COLUMN!
        };
        transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
        std::list<std::vector<double> > pointsForTriangle;

        for(auto i = pointsForHistogram.begin(); i != pointsForHistogram.end(); ){
            glm::dvec3 point = {i->at(0), i->at(1), i->at(2)};
            if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
               //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
               pointsForTriangle.push_back(*i);
               i = pointsForHistogram.erase(i);
            }else{
               ++i;
            }
        }

        subdivide(v1, v2, v3, numberOfSubdivisions, pointsForTriangle);
    }
}

void Icosphere::subdivide(double *v1, double *v2, double *v3, long depth, std::list<std::vector<double> > &allRemainingPoints)
// ACHTUNG: GEMEINSAME LISTE DER PUNKTE FUER ALLE DREIECKE, AUS DER BEIM MALEN GELÖSCHT WIRD. Funktioniert nur, solange nicht parallel.
{
   if (depth == 0) {
      triangleColor(allRemainingPoints.size());
      ////triangleColor(allRemainingPoints.size());
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      drawTriangle(v1, v2, v3);


      // Draw triangle outlines:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glColor4f(0.8784313725490196, 0.9529411764705882, 0.9725490196078431, 0.2);
      double v1l[3] = {v1[0] * 1.001, v1[1] * 1.001, v1[2] * 1.001};
      double v2l[3] = {v2[0] * 1.001, v2[1] * 1.001, v2[2] * 1.001};
      double v3l[3] = {v3[0] * 1.001, v3[1] * 1.001, v3[2] * 1.001};

      drawTriangle(v1l, v2l, v3l);

      return;
   }

   GLdouble v12[3], v23[3], v31[3];
   GLint i;
   std::list<std::vector<double> > pointsInTriangle;

   for (i = 0; i < 3; i++) {
      v12[i] = v1[i]+v2[i];
      v23[i] = v2[i]+v3[i];
      v31[i] = v3[i]+v1[i];
   }
   normalize(v12);
   normalize(v23);
   normalize(v31);

// Hässliches Copy-Paste für die 4 verschiedenen Unterdreiecke:
//// TODO: IN METHODE AUSLAGERN

// Dreieck 1
   glm::dmat3 transformationMatrix = {
       v1[0], v1[1], v1[2],     // first COLUMN!
       v12[0], v12[1], v12[2],     // second COLUMN!
       v31[0], v31[1], v31[2]     // third COLUMN!
   };
   transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
   std::list<std::vector<double> > pointsForTriangle;

   for(auto i = allRemainingPoints.begin(); i != allRemainingPoints.end(); ){
       glm::dvec3 point = {i->at(0), i->at(1), i->at(2)};
       if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
          //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
          pointsForTriangle.push_back(*i);
          i = allRemainingPoints.erase(i);
       }else{
          ++i;
       }
   }
   subdivide(v1, v12, v31, depth-1, pointsForTriangle);

// Dreieck 2
   transformationMatrix = {
       v2[0], v2[1], v2[2],     // first COLUMN!
       v23[0], v23[1], v23[2],     // second COLUMN!
       v12[0], v12[1], v12[2]     // third COLUMN!
   };
   transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
   pointsForTriangle.clear();

   for(auto i = allRemainingPoints.begin(); i != allRemainingPoints.end(); ){
       glm::dvec3 point = {i->at(0), i->at(1), i->at(2)};
       if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
          //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
          pointsForTriangle.push_back(*i);
          i = allRemainingPoints.erase(i);
       }else{
          ++i;
       }
   }
   subdivide(v2, v23, v12, depth-1, pointsForTriangle);

// Dreieck 3
  transformationMatrix = {
      v3[0], v3[1], v3[2],     // first COLUMN!
      v31[0], v31[1], v31[2],     // second COLUMN!
      v23[0], v23[1], v23[2]     // third COLUMN!
  };
  transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
  pointsForTriangle.clear();

  for(auto i = allRemainingPoints.begin(); i != allRemainingPoints.end(); ){
      glm::dvec3 point = {i->at(0), i->at(1), i->at(2)};
      if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
         //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
         pointsForTriangle.push_back(*i);
         i = allRemainingPoints.erase(i);
      }else{
         ++i;
      }
  }
   subdivide(v3, v31, v23, depth-1, pointsForTriangle);

// Dreieck 4
      transformationMatrix = {
          v12[0], v12[1], v12[2],     // first COLUMN!
          v23[0], v23[1], v23[2],     // second COLUMN!
          v31[0], v31[1], v31[2]     // third COLUMN!
      };
      transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates
      pointsForTriangle.clear();

      for(auto i = allRemainingPoints.begin(); i != allRemainingPoints.end(); ){
          glm::dvec3 point = {i->at(0), i->at(1), i->at(2)};
          if(pointInFirstQuadrantAfterTransformation(point , transformationMatrix)){
             //std::cout << "(" << i->at(0) << ", " << i->at(1) << ", " << i->at(2) << ")" << std::endl;
             pointsForTriangle.push_back(*i);
             i = allRemainingPoints.erase(i);
          }else{
             ++i;
          }
      }
   subdivide(v12, v23, v31, depth-1, pointsForTriangle);
}

void Icosphere::triangleColor(unsigned long pointsInTriangle){

    for(std::map<size_t, glm::vec3>::iterator it = colorLookupMap.begin(); it != colorLookupMap.end(); ++it){
        if(std::next(it) == colorLookupMap.end() || (it->first <= pointsInTriangle && std::next(it)->first > pointsInTriangle)){
            glColor4f(it->second[0], it->second[1], it->second[2], 0.5);
            break;
        }
    }
}

bool Icosphere::pointInFirstQuadrantAfterTransformation(const glm::dvec3 &point, const glm::dmat3 &transformationMatrix){

    glm::dvec3 transformedPoint = transformationMatrix * point;             // point in coordinate system of triangle vertices

    if(transformedPoint[0] >= 0 && transformedPoint[1] >= 0 && transformedPoint[2] >= 0){
        return true;
    }else{
        return false;
    }

}

