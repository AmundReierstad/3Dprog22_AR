#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H
#include"visualobject.h"
#include"pickup.h"

class OctahedronBall: public VisualObject
{
public:

    OctahedronBall(int n=0, float radius=1);// n rekursjoner
    ~OctahedronBall();
    void init(GLint matrixUniform) override;
    void draw() override;
    bool checkCollision( Pickup& other);
    float radius;
private:
    int m_rekursjoner;
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
    void lagTriangel(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
    //constructor subfunction
    void oktaederUnitBall();
    void move( float x, float y, float z) override;
    float mx, my, mz; //positions for move functions

};

#endif // OCTAHEDRONBALL_H
