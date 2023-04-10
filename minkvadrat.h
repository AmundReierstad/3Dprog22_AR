#ifndef MINKVADRAT_H
#define MINKVADRAT_H
#include"visualobject.h"

class MinKvadrat: public VisualObject
{
public:
    MinKvadrat();

public:
    ~MinKvadrat() override;
    MinKvadrat(std::string filnavn);


   void readFile(std::string filnavn);
   void init(GLint matrixUniform) override;
   void draw() override;
};

#endif // MINKVADRAT_H
