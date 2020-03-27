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
}

void Icosphere::drawIcosphere(unsigned int numberOfSubdivisions, std::vector<std::vector<double> > points){
//    if(numberOfSubdivisions == 0){
//        glBegin(GL_TRIANGLES);
//        for (size_t i = 0; i < indices.size(); i++) {

//               glNormal3d(  vertices[indices[i][0]] [0],
//                            vertices[indices[i][0]] [1],
//                            vertices[indices[i][0]] [2]);
//               glVertex3d(  vertices[indices[i][0]] [0],
//                            vertices[indices[i][0]] [1],
//                            vertices[indices[i][0]] [2]);
//               glNormal3d(  vertices[indices[i][1]] [0],
//                            vertices[indices[i][1]] [1],
//                            vertices[indices[i][1]] [2]);
//               glVertex3d(  vertices[indices[i][1]] [0],
//                            vertices[indices[i][1]] [1],
//                            vertices[indices[i][1]] [2]);
//               glNormal3d(  vertices[indices[i][2]] [0],
//                            vertices[indices[i][2]] [1],
//                            vertices[indices[i][2]] [2]);
//               glVertex3d(  vertices[indices[i][2]] [0],
//                            vertices[indices[i][2]] [1],
//                            vertices[indices[i][2]] [2]);
//        }
//        glEnd();
//    }
    pointsOnTriangleBoundary = 0;

    for (std::vector<double> &i : indices){
        double v1[3] = {vertices[i[0]][0], vertices[i[0]][1], vertices[i[0]][2]};
        double v2[3] = {vertices[i[1]][0], vertices[i[1]][1], vertices[i[1]][2]};
        double v3[3] = {vertices[i[2]][0], vertices[i[2]][1], vertices[i[2]][2]};
        subdivide(v1, v2, v3, numberOfSubdivisions);
    }



}
void Icosphere::subdivide(double *v1, double *v2, double *v3, long depth)
{
   GLdouble v12[3], v23[3], v31[3];
   GLint i;

   if (depth == 0) {
      drawTriangle(v1, v2, v3);
      return;
   }
   for (i = 0; i < 3; i++) {
      v12[i] = v1[i]+v2[i];
      v23[i] = v2[i]+v3[i];
      v31[i] = v3[i]+v1[i];
   }
   normalize(v12);
   normalize(v23);
   normalize(v31);
   subdivide(v1, v12, v31, depth-1);
   subdivide(v2, v23, v12, depth-1);
   subdivide(v3, v31, v23, depth-1);
   subdivide(v12, v23, v31, depth-1);
}

void Icosphere::triangleColor(unsigned long pointsInTriangle){
    if(pointsInTriangle == 0){
        glColor3s(69,117,180);
    }
}

bool Icosphere::pointInTriangleRange(const GLdouble point[3], const GLdouble triangleVertices[3][3] ){
    glm::dmat3 transformationMatrix = {
        triangleVertices[0][0], triangleVertices[0][1], triangleVertices[0][2],     // first COLUMN!
        triangleVertices[1][0], triangleVertices[1][1], triangleVertices[1][2],     // second COLUMN!
        triangleVertices[2][0], triangleVertices[2][1], triangleVertices[2][2],     // third COLUMN!
    };
    transformationMatrix = glm::inverse(transformationMatrix);          // transformation matrix inversed in order to change basis to triangle vertices' coordinates

    glm::dvec3 p = {point[0], point[1], point[2]};
    glm::dvec3 transformedPoint = transformationMatrix * p;             // point in coordinate system of triangle vertices

    if(transformedPoint[0] >= 0 && transformedPoint[1] >= 0 && transformedPoint[2] >= 0){
        if(transformedPoint[0] == 0 || transformedPoint[1] == 0 || transformedPoint[2] == 0){
            ++pointsOnTriangleBoundary;
        }
        return true;
    }
    return false;
}

