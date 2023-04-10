#ifndef PICKUP_H
#define PICKUP_H
#include"visualobject.h"

class Pickup: public VisualObject
{
public:
    Pickup(int n, float scaleInp);
    ~Pickup();
    void init(GLint matrixUniform) override;
    void draw() override;
    float radius;
    bool hasCollided;
    void onCollision();
private:
    int m_rekursjoner;
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
    void lagTriangel(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
    void oktaederUnitBall();
    void move(float x, float y, float z) override;
//    void move(float x, float y, float z) override;
//    void move(float dt) override;
};

#endif // PICKUP_H
