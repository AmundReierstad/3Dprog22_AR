#ifndef DOOR_H
#define DOOR_H
#include"visualobject.h"
#include"octahedronball.h"

class Door: public VisualObject
{
public:
    Door();
    ~Door() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move( float x, float y, float z) override;
    void onCollision(bool isColliding);
    bool checkCollision(OctahedronBall& player);
    float activationTreshold;

private:
    bool isOpen;
};

#endif // DOOR_H
