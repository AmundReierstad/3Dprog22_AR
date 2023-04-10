#include "door.h"

Door::Door(): activationTreshold(1)
{

    //will be in ZY plane
    mVertices.push_back(Vertex{0,0,0.5,1,0,0});
    mVertices.push_back(Vertex{0,0,-0.5,1,0,0});
    mVertices.push_back(Vertex{0,2,-0.5,1,0,0});

    mVertices.push_back(Vertex{0,2,-0.5,1,0,0});
    mVertices.push_back(Vertex{0,2,0.5,1,0,0});
    mVertices.push_back(Vertex{0,0,0.5,1,0,0});
}

Door::~Door()
{

}

void Door::init(GLint matrixUniform)
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

void Door::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Door::move(float x, float y, float z)
{
    mPosition.setToIdentity(); // hvis nødvendig
    QVector4D pos{x,y,z,1.0};
    mPosition.setColumn(3,pos); //matrisene er transponerte, derav row og ikke col
    mMatrix = mPosition;
}

void Door::onCollision(bool isColliding)
{
    if(!isOpen &&isColliding)
    {mMatrix.rotate(90,{0,1,0}); isOpen=true; return;}

    if(!isColliding && isOpen)
    {mMatrix.rotate(-90,{0,1,0}); isOpen=false;}
}

bool Door::checkCollision(OctahedronBall& player)
{
    float length;
    length=(mMatrix.column(3)-player.mMatrix.column(3)).length(); //distance between actors
   if( length<=activationTreshold+player.radius)
   return true;
             else  return false;
}
