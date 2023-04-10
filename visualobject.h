// 3D-programmering 2022
#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
   VisualObject();
   ~VisualObject();
   virtual void init(GLint matrixUniform)=0;
   virtual void draw()=0;
   virtual void move( float x, float y, float z){};
   virtual void move( float dt){};
   std::pair<float,float> getPosition2D();
   void setName(std::string name);
   std::string getName();
   QMatrix4x4 mMatrix;

protected:
   std::string mName;
   std::vector<Vertex> mVertices;
   GLuint mVAO{0};
   GLuint mVBO{0};
   GLint mMatrixUniform{0};

   QMatrix4x4 mPosition;
   QMatrix4x4 mRotation;
   QMatrix4x4 mScale;

   QVector3D mVelocity;

};
#endif // VISUALOBJECT_H
