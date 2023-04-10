#include "octahedronball.h"


OctahedronBall::OctahedronBall(int n, float radius):VisualObject(), radius(radius), m_rekursjoner(n), m_indeks(0)
{
    mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall();

}

OctahedronBall::~OctahedronBall()
{

}

void OctahedronBall::init(GLint matrixUniform)
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

void OctahedronBall::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

bool OctahedronBall::checkCollision( Pickup &other)
{
    float length;
    length=(mMatrix.column(3)-other.mMatrix.column(3)).length();
   if( length<=this->radius+other.radius)
       return true;
   else return false;
}



void OctahedronBall::lagTriangel(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    Vertex v{v1.x(), v1.y(), v1.z(), v1.x(), v1.y(), v1.z()};
    mVertices.push_back(v);
    v = Vertex{v2.x(), v2.y(), v2.z(), v2.x(), v2.y(), v2.z()};
    mVertices.push_back(v);
    v = Vertex{v3.x(), v3.y(), v3.z(), v3.x(), v3.y(), v3.z()};
    mVertices.push_back(v);
}

void OctahedronBall::subDivide(const QVector3D &a, const QVector3D &b, const QVector3D &c, int n)
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
//constructor subfunction
void OctahedronBall::oktaederUnitBall()
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

void OctahedronBall::move(float dx, float dy, float dz)
{
    mx+=dx;
    my+=dy;
    mz+=dz;
    mMatrix.setToIdentity();
    mMatrix.translate(mx,my,mz);
}

