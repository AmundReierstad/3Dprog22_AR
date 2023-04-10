#include "light.h"

light::light()
{
    //plane XY
    Vertex v0{0.0,0.0,0.0, 1,0,0};    mVertices.push_back(v0);
    Vertex v1(0.5,0.0,0.0, 0,1,0);    mVertices.push_back(v1);
    Vertex v2{0.5,0.5,0.0, 0,0,1};    mVertices.push_back(v2);
    Vertex v3{0.0,0.0,0.0, 0,0,1};    mVertices.push_back(v3);
    Vertex v4{0.5,0.5,0.0, 0,1,0};    mVertices.push_back(v4);
    Vertex v5{0.0,0.5,0.0, 1,0,0};    mVertices.push_back(v5);
    // plane XY transposed 0.5Z
    mVertices.push_back(Vertex{0,0,0.5,1,0,0});
    mVertices.push_back(Vertex{0.5,0,0.5,0,1,0});
    mVertices.push_back(Vertex{0.5,0.5,0.5,0,0,1});
    mVertices.push_back(Vertex{0,0,0.5,0,0,1});
    mVertices.push_back(Vertex{0.5,0.5,0.5,0,1,0});
    mVertices.push_back(Vertex{0,0.5,0.5,1,0,0});

    //plane top XZ, 0.5 Y
    mVertices.push_back(Vertex{0,0.5,0,1,0,0});
    mVertices.push_back(Vertex{0.5,0.5,0,1,0,0});
    mVertices.push_back(Vertex{0.5,0.5,0.5,1,0,0});
    mVertices.push_back(Vertex{0,0.5,0,1,0,0});
    mVertices.push_back(Vertex{0,0.5,0.5,1,0,0});
    mVertices.push_back(Vertex{0.5,0.5,0.5,1,0,0});
    //plane bottom XZ, 0 Y
    mVertices.push_back(Vertex{0,0,0,1,0,0});
    mVertices.push_back(Vertex{0.5,0,0,1,0,0});
    mVertices.push_back(Vertex{0.5,0,0.5,1,0,0});
    mVertices.push_back(Vertex{0,0,0,1,0,0});
    mVertices.push_back(Vertex{0,0,0.5,1,0,0});
    mVertices.push_back(Vertex{0.5,0,0.5,1,0,0});
    //plane YZ
    mVertices.push_back(Vertex{0,0,0,0,1,0});
    mVertices.push_back(Vertex{0,0,0.5,0,1,0});
    mVertices.push_back(Vertex{0,0.5,0.5,0,1,0});
    mVertices.push_back(Vertex{0,0,0,0,1,0});
    mVertices.push_back(Vertex{0,0.5,0.5,0,1,0});
    mVertices.push_back(Vertex{0,0.5,0,0,1,0});
    //plane YZ transposed 0.5X
    mVertices.push_back(Vertex{0.5,0,0,1,1,0});
    mVertices.push_back(Vertex{0.5,0,0.5,1,1,0});
    mVertices.push_back(Vertex{0.5,0.5,0.5,1,1,0});
    mVertices.push_back(Vertex{0.5,0,0,1,1,0});
    mVertices.push_back(Vertex{0.5,0.5,0.5,1,1,0});
    mVertices.push_back(Vertex{0.5,0.5,0,1,1,0});

    //set
    mMatrix.setToIdentity();

    rot= 0.f;
}

light::~light()
{

}

void light::init(GLint matrixUniform)
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

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void light::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void light::move(float dx, float dy, float dz)
{
    mx+=dx;
    my+=dy;
    mz+=dz;
    mMatrix.setToIdentity();
    mMatrix.translate(mx,my,mz);
}

void light::move(float dt)
{

    this->mMatrix.translate(sinf(rot*dt)/100, cosf(rot*dt)/100,cosf(rot*dt)/100);
    rot+=0.01f;
    curPos=this->mMatrix.column(3).toVector3D();

}
