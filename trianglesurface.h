#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
#include<fstream>
#include"stb_image.h"

class TriangleSurface : public VisualObject
{

    public:
       TriangleSurface();
       TriangleSurface(std::string filnavn);
      ~TriangleSurface() override;

       void readFile(std::string filnavn);
       void init(GLint matrixUniform) override;
       void draw() override;

    private:
       GLuint texture1;
};

#endif // TRIANGLESURFACE_H
