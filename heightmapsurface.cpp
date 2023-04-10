#include "heightmapsurface.h"
#include<iostream>
#include<iterator>

HeightMapSurface::HeightMapSurface(const char* imagePath, float scale)
{
    //load image to heightMap

    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

    // vertex generation
    int rez = 1; //resolutuon for indices
    unsigned bytePerPixel = nrChannels;
    float yScale = 10.f / 256.0f, yShift = 5.9f;  // apply a scale+shift to the height data
    //will have size width*heigth*3 elements
    for( int i = 0; i < height; i++)
    {
        for( int j = 0; j < width; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;//1 byte 0-255
            // raw height at coordinate
            unsigned char rawHeight = pixelOffset[0];

            float x=(-height/2.0f + height*i / (float)height)*scale;
            float y=( (int)rawHeight * yScale - yShift);
            float z=( -width/2.0f + width*j / (float)width)*scale;
            // push back vertex, dataa
            mVertices.push_back( Vertex{x,y,z,(int)rawHeight/255.f,(int)rawHeight/255.f,(int)rawHeight/255.f });    //rgb depdendent on height, gives flat shading after pipeline
        }
    }

    //free memory after calculating vertexes
    stbi_image_free(data);

    //compute indices for strip generation
    for( int i = 0; i < height-1; i+=rez)       // for each row a.k.a. each strip
    {
        for( int j = 0; j < width; j+=rez)      // for each column
        {
            for( int k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(j + width * (i + k*rez));
            }
        }
    }
         NUM_STRIPS = (height-1)/rez;
         NUM_VERTS_PER_STRIP = (width/rez)*2-2;

     mMatrix.setToIdentity();

     //need these for search optimization later on, when searching for triangle containing player.
     prevIndexHeigth=3;
     prevIndexWith=3;
}

HeightMapSurface::~HeightMapSurface()
{

}

void HeightMapSurface::init(GLint matrixUniform)
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

    // 1rst attribute buffer :position
//    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // EBO, element buffer object
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int), // size of indices buffer
                 &indices[0],                           // pointer to first element
                 GL_STATIC_DRAW);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void HeightMapSurface::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

    for(unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                          NUM_VERTS_PER_STRIP, // number of indices to render
                          GL_UNSIGNED_INT,     // index data type
                         (void*)(sizeof(unsigned) * (NUM_VERTS_PER_STRIP+2) * strip));; // offset to starting index
    }
//    glBindVertexArray( mVAO );
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void HeightMapSurface::CheckPlayerHeight(OctahedronBall& player)
{
    QVector3D playerPosition=player.mMatrix.column(3).toVector3D();
    QVector3D barys{0,0,0};
    QVector4D temp=player.mMatrix.column(3);
/*    QVector3D test=mVertices.at(3000).getXYZ();*/ //this returns actual vertex data
    float heightPlayer{-1};
    //loop till we find which triangle we are in in the XZ plane:

    //change iteration to a for loop, pointer iteration does not work need to build this so the triangle makes sense as in the strip when calc the barycentric cords
         for (int i=prevIndexHeigth-1; i< height-2; i++) {          // issues here, does not get the right pointers to the vertexes

             for(int j=prevIndexWith-1;j<width-2;j++){
                 //top of strip check
                 barys=returnBarycentricCoordinatesXZplane(playerPosition,mVertices.at(width*i+j).getXYZ(),mVertices.at(width*(i+1)+width).getXYZ(),mVertices.at(width*i+(j+1)).getXYZ());
                  if (barys.x()>=0 && barys.y()>=0 && barys.z()>=0){
                      //if triangle is found, adjust height of player to location on triangle
                        heightPlayer=barys.x()*mVertices.at(width*i+j).getXYZ().y()
                                    +barys.y()*mVertices.at(width*(i+1)+width).getXYZ().y()
                                    +barys.z()*mVertices.at(width*i+(j+1)).getXYZ().y();


                        temp.setY(heightPlayer);
                        player.mMatrix.setToIdentity();
                        player.mMatrix.setColumn(3,temp);
                        prevIndexHeigth=i; prevIndexWith=j;
                        return;
                   //bottom of strip check
                 }

                  barys=returnBarycentricCoordinatesXZplane(playerPosition,mVertices.at(width*i+(j+1)).getXYZ(),mVertices.at(width*(i+1)+width).getXYZ(),mVertices.at(width*(i+1)+(j+1)).getXYZ());
                   if (barys.x()>=0 && barys.y()>=0 && barys.z()>=0) {
                       //if triangle is found, adjust height of player to location on triangle
                         heightPlayer=barys.x()*mVertices.at(width*i+(j+1)).getXYZ().y()
                                     +barys.y()*mVertices.at(width*(i+1)+width).getXYZ().y()
                                     +barys.z()*mVertices.at(width*(i+1)+(j+1)).getXYZ().y();


                         temp.setY(heightPlayer);
                         player.mMatrix.setToIdentity();
                         player.mMatrix.setColumn(3,temp);
                         prevIndexHeigth=i; prevIndexWith=j;
                         return;
                  }
              }
         }
//           player.mMatrix.setColumn(3,temp);
         return; //if not found, do nothing
}

QVector3D HeightMapSurface::returnBarycentricCoordinatesXZplane(QVector3D point,QVector3D P,QVector3D Q, QVector3D R)
{
    //check in relation to XZ plane, therefore set y to 0 for all points, we will try to find which triangle player pos corresponds too.
    point.setY(0);  P.setY(0);  Q.setY(0);  R.setY(0);
    QVector3D barycentricCoordinates{0,0,0};
    QVector3D temp;

    QVector3D pointPvec=P-point;
    QVector3D pointQvec=Q-point;
    QVector3D pointRvec=R-point;

    QVector3D PQ=Q-P;
    QVector3D PR=R-P;
    temp=temp.crossProduct(PQ,PR);
    float areaTriangle= temp.y();

    // U cord, P influence
    temp=temp.crossProduct(pointQvec,pointRvec);
    float areaU=temp.y();
    barycentricCoordinates.setX(areaU/areaTriangle);
    // V cord, Q influence
    temp=temp.crossProduct(pointRvec,pointPvec);
    float areaV=temp.y();
    barycentricCoordinates.setY(areaV/areaTriangle);
    // W cord, R influence
    temp=temp.crossProduct(pointPvec,pointQvec);
    float areaW=temp.y();
    barycentricCoordinates.setZ(areaW/areaTriangle);



    return barycentricCoordinates;
}


