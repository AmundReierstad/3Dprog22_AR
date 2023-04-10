#include "cube.h"
#include<iostream>
cube::cube()
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

    //graph travel paths:
    x=0;
    increaseX=true;
    polynomMatrix=QMatrix4x4(0.f,0.f,0.f,1.f,
                             1.f,1.f,1.f,1.f,
                             8.f,4.f,2.f,1.f,
                             27.f,9.f,3.f,1.f);
    polynomMatrixInv=polynomMatrix.inverted();
    Y={0,-1,1,2};
    polynom=polynomMatrixInv*Y;
    doParabel=true;

}
cube::~cube(){}
void cube::init(GLint matrixUniform)
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
};
void cube::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void cube::move(float dx, float dy, float dz)
{
    mx+=dx;
    my+=dy;
    mz+=dz;
    mMatrix.setToIdentity();
    mMatrix.translate(mx,my,mz);
}

//trengs mer arbeid, fungerer syklisk men funksjonsverdien langs y blir rare
void cube::move(float dt)
{
    float y{};
    if(x>=2){increaseX=false;}
    if(x<=-2){increaseX=true;}

    if(increaseX)x+=0.01*dt;
            else x-=0.01*dt;

    if(doParabel)
        y=pow(x,2);
     if(!doParabel) //looks kind of fucky with camera centered on ball player, can try other polynom
        y= (pow(x*polynom.x(),3)+pow(x*polynom.y(),2)+x*polynom.z()+polynom.w());

    mPosition.setToIdentity(); // hvis nødvendig
    QVector4D pos{x,y,-2,1.0};
    mPosition.setColumn(3,pos); //matrisene er transponerte, derav row og ikke col
    mMatrix = mPosition;
//    mMatrix.setToIdentity();
//    mMatrix.translate(functionXvalue,functionYvalue,0);
};
