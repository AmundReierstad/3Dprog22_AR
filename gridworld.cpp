#include "gridworld.h"

GridWorld::GridWorld(const float scale)
{
    //XY kvadrat z+
    mVertices.push_back(Vertex{scale,scale,scale,1,1,1});
    mVertices.push_back(Vertex{scale,-scale,scale,1,1,1});

    mVertices.push_back(Vertex{scale,-scale,scale,1,1,1});
    mVertices.push_back(Vertex{-scale,-scale,scale,1,1,1});

    mVertices.push_back(Vertex{scale,scale,scale,1,1,1});
    mVertices.push_back(Vertex{-scale,scale,scale,1,1,1});

    mVertices.push_back(Vertex{-scale,scale,scale,1,1,1});
    mVertices.push_back(Vertex{-scale,-scale,scale,1,1,1});
    //XY kvadrat z-
    mVertices.push_back(Vertex{scale,scale,-scale,1,1,1});
    mVertices.push_back(Vertex{scale,-scale,-scale,1,1,1});

    mVertices.push_back(Vertex{scale,-scale,-scale,1,1,1});
    mVertices.push_back(Vertex{-scale,-scale,-scale,1,1,1});

    mVertices.push_back(Vertex{scale,scale,-scale,1,1,1});
    mVertices.push_back(Vertex{-scale,scale,-scale,1,1,1});

    mVertices.push_back(Vertex{-scale,scale,-scale,1,1,1});
    mVertices.push_back(Vertex{-scale,-scale,-scale,1,1,1});

    //lines between kvadrats
    mVertices.push_back(Vertex{scale,-scale,+scale,1,1,1});
    mVertices.push_back(Vertex{scale,-scale,-scale,1,1,1});

    mVertices.push_back(Vertex{scale,scale,+scale,1,1,1});
    mVertices.push_back(Vertex{scale,scale,-scale,1,1,1});

    mVertices.push_back(Vertex{-scale,scale,+scale,1,1,1});
    mVertices.push_back(Vertex{-scale,scale,-scale,1,1,1});

    mVertices.push_back(Vertex{-scale,-scale,+scale,1,1,1});
    mVertices.push_back(Vertex{-scale,-scale,-scale,1,1,1});
}

GridWorld::~GridWorld()
{

}

void GridWorld::init(GLint matrixUniform)
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

void GridWorld::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());
}
