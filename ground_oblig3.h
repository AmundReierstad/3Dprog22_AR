#ifndef GROUND_OBLIG3_H
#define GROUND_OBLIG3_H
#include"visualobject.h"

class ground_oblig3: public VisualObject
{
public:
    ground_oblig3();
   ~ground_oblig3() override;



private:
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // GROUND_OBLIG3_H
