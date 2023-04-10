#ifndef HOUSE_H
#define HOUSE_H
#include"visualobject.h"

class House: public VisualObject
{
public:
    House();
    ~House() override;
   void init(GLint matrixUniform) override;
   void draw() override;
   void move( float x, float y, float z) override;
private:
   float mx, my, mz; //positions
   double functionXvalue;
   bool increaseX;
};

#endif // HOUSE_H
