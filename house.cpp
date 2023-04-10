#include "house.h"

House::House()
{
    //front XY with door slot
        //lower half L
     mVertices.push_back(Vertex{0,0,0,1,1,0});
     mVertices.push_back(Vertex{1.5f,0,0,1,1,0});
     mVertices.push_back(Vertex{1.5f,2,0,1,1,0});
     mVertices.push_back(Vertex{0,0,0,1,1,0});
     mVertices.push_back(Vertex{1.5f,2,0,1,1,0});
     mVertices.push_back(Vertex{0,2,0,1,1,0});
        //lower half R
     mVertices.push_back(Vertex{2.5f,0,0,1,1,0});
     mVertices.push_back(Vertex{4,0,0,1,1,0});
     mVertices.push_back(Vertex{2.5f,2,0,1,1,0});
     mVertices.push_back(Vertex{4,0,0,1,1,0});
     mVertices.push_back(Vertex{4,2,0,1,1,0});
     mVertices.push_back(Vertex{2.5,2,0,1,1,0});
        //top XY
     mVertices.push_back(Vertex{0,2,0,1,1,0});
     mVertices.push_back(Vertex{4,2,0,1,1,0});
     mVertices.push_back(Vertex{0,4,0,1,1,0});
     mVertices.push_back(Vertex{0,4,0,1,1,0});
     mVertices.push_back(Vertex{4,4,0,1,1,0});
     mVertices.push_back(Vertex{4,2,0,1,1,0});

    //side XZ L
    mVertices.push_back(Vertex{0,0,0,1,0.8,0});
    mVertices.push_back(Vertex{0,0,-4,1,0.8,0});
    mVertices.push_back(Vertex{0,4,0,1,0.8,0});
    mVertices.push_back(Vertex{0,0,-4,1,0.8,0});
    mVertices.push_back(Vertex{0,4,-4,1,0.8,0});
    mVertices.push_back(Vertex{0,4,0,1,0.8,0});
    //side XZ R
    mVertices.push_back(Vertex{4,0,0,1,0.8,0});
    mVertices.push_back(Vertex{4,0,-4,1,0.8,0});
    mVertices.push_back(Vertex{4,4,0,1,0.8,0});
    mVertices.push_back(Vertex{4,0,-4,1,0.8,0});
    mVertices.push_back(Vertex{4,4,-4,1,0.8,0});
    mVertices.push_back(Vertex{4,4,0,1,0.8,0});

    mMatrix.setToIdentity();

    //back XY
    mVertices.push_back(Vertex{0,0,-4,1,0.5,0});
    mVertices.push_back(Vertex{4,0,-4,1,0.5,0});
    mVertices.push_back(Vertex{0,4,-4,1,0.5,0});
    mVertices.push_back(Vertex{0,4,-4,1,0.5,0});
    mVertices.push_back(Vertex{4,0,-4,1,0.5,0});
    mVertices.push_back(Vertex{4,4,-4,1,0.5,0});

    //floor XZ
    mVertices.push_back(Vertex{4,0,0,1,0.3,0});
    mVertices.push_back(Vertex{0,0,0,1,0.3,0});
    mVertices.push_back(Vertex{0,0,-4,1,0.3,0});
    mVertices.push_back(Vertex{4,0,0,1,0.3,0});
    mVertices.push_back(Vertex{0,0,-4,1,0.3,0});
    mVertices.push_back(Vertex{4,0,-4,1,0.3,0});

}

House::~House()
{

}

void House::init(GLint matrixUniform)
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

void House::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void House::move(float x, float y, float z)
{
    QVector4D pos{x,y,z,1.0};
    mPosition.setColumn(3,pos); //matrisene er transponerte, derav row og ikke col
    mMatrix = mPosition; // * mScale * mRotation etter behov, pass på rekkefølgen
}

