#ifndef CUBE_H
#define CUBE_H
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include"visualobject.h"
#include"stb_image.h"

class cube : public VisualObject
{
public:
    cube();
    ~cube() override;
   void init(GLint matrixUniform) override;
   void draw() override;
   void move( float x, float y, float z) override;
   void move( float dt) override;
   bool doParabel;
private:
   float mx, my, mz; //positions
   float x;
   bool increaseX;
   //needed for calculating equation for polynomial travel path:
   QMatrix4x4 polynomMatrix;
   QMatrix4x4 polynomMatrixInv;
   QVector4D Y;
   QVector4D polynom;
};

#endif // CUBE_H
