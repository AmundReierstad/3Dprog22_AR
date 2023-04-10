#include "ground_oblig3.h"

ground_oblig3::ground_oblig3()
{
    //XZ plane
    Vertex v0{5.0,-0.1,5.0, 0,1,0};    mVertices.push_back(v0);
    Vertex v1(5.0,-0.1,-5.0, 0,1,0);      mVertices.push_back(v1);
    Vertex v2{-5.0,-0.1,5.0, 0,1,0};      mVertices.push_back(v2);
    Vertex v3{-5.0,-0.1,5.0, 0,1,0};    mVertices.push_back(v3);
    Vertex v4{-5.0,-0.1,-5.0, 0,1,0};     mVertices.push_back(v4);
    Vertex v5{5.0,-0.1,-5.0, 0,1,0};  mVertices.push_back(v5);

    mMatrix.setToIdentity();
}

ground_oblig3::~ground_oblig3()
{

}

void ground_oblig3::init(GLint matrixUniform)
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

void ground_oblig3::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}
