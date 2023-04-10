#ifndef HEIGHTMAPSURFACE_H
#define HEIGHTMAPSURFACE_H
#include"visualobject.h"
#include"stb_image.h"
#include"octahedronball.h"
class HeightMapSurface: public VisualObject
{
public:
    HeightMapSurface(const char* imagePath,float scale);
    ~HeightMapSurface() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void CheckPlayerHeight(OctahedronBall& player);
    QVector3D returnBarycentricCoordinatesXZplane(QVector3D point,QVector3D P,QVector3D Q, QVector3D R);
private:
       //for triangle strip generation
       std::vector<unsigned int> indices;
        unsigned int NUM_STRIPS;
        unsigned int NUM_VERTS_PER_STRIP;
        OctahedronBall* PlayerRef;
        GLuint mEBO{0}; //element buffer object, for triangle strips
        int width, height, nrChannels;
        int prevIndexWith, prevIndexHeigth;
};

#endif // HEIGHTMAPSURFACE_H
