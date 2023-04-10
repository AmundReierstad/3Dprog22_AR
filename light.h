#ifndef LIGHT_H
#define LIGHT_H
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include"visualobject.h"
class light: public VisualObject
{
public:
    light();
    ~light() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move( float x, float y, float z) override;
    void move( float dt) override;
    QVector3D curPos;
private:
    float mx, my, mz; //positions
    float rot;
    bool increaseX;
};

#endif // LIGHT_H
