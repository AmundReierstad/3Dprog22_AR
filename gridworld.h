#ifndef GRIDWORLD_H
#define GRIDWORLD_H
#include"visualobject.h"


//Grid to showborders of the gameworld for easier orentation
class GridWorld: public VisualObject
{
public:
    GridWorld(const float scale);
    ~GridWorld() override;

    void init(GLint matrixUniform) override;
    void draw() override;
private:
};

#endif // GRIDWORLD_H
