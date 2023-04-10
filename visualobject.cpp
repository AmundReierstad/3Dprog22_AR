// 3D-programmering 2022
#include "visualobject.h"

VisualObject::VisualObject() {  }
VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

std::pair<float, float> VisualObject::getPosition2D()
{
    auto col= mPosition.column(3);
    return std::pair<float, float>(col.x(),col.y());
}

void VisualObject::setName(std::string name)
{
    mName=name;
}

std::string VisualObject::getName()
{
   return mName;
}
