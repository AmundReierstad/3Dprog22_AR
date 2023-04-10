#include "pickup.h"
#include"QDebug"

Pickup::Pickup(int n, float scaleInp):VisualObject(),m_rekursjoner(n), radius(scaleInp), m_indeks(0)
{
    mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall();
    hasCollided=false;
}

Pickup::~Pickup()
{

}

void Pickup::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Pickup::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Pickup::lagTriangel(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    Vertex v{v1.x(), v1.y(), v1.z(), 1, 1, 1};
    mVertices.push_back(v);
    v = Vertex{v2.x(), v2.y(), v2.z(), 1, 1, 1};
    mVertices.push_back(v);
    v = Vertex{v3.x(), v3.y(), v3.z(), 1, 1, 1};
    mVertices.push_back(v);
}

void Pickup::subDivide(const QVector3D &a, const QVector3D &b, const QVector3D &c, int n)
{
    if (n>0) {
        QVector3D v1 = a+b; v1.normalize();v1=v1*radius; //for smaller radius
        QVector3D v2 = a+c; v2.normalize();v2=v2*radius;
        QVector3D v3 = c+b; v3.normalize(); v3=v3*radius;
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    } else {
        lagTriangel(a, b, c);
    }
}

void Pickup::oktaederUnitBall()
{
    QVector3D v0{0, 0, radius}; //set to 0,5 for smaller radius, as in subdiv
    QVector3D v1{radius, 0, 0};
    QVector3D v2{0, radius, 0};
    QVector3D v3{-radius, 0, 0};
    QVector3D v4{0, -radius, 0};
    QVector3D v5{0, 0, -radius};


    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);
}

void Pickup::move(float x, float y, float z)
{
    mPosition.setToIdentity(); // hvis nødvendig
    QVector4D pos{x,y,z,1.0};
    mPosition.setColumn(3,pos); //matrisene er transponerte, derav row og ikke col
    mMatrix = mPosition; // * mScale * mRotation etter behov, pass på rekkefølgen
}

void Pickup::onCollision()
{
    //scale down on collision, tried to change color, but vertexes are bound, check more into this/ask ( )
    mMatrix.scale({0.1,0.1,0.1});
    hasCollided=true;
}
