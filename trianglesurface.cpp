#include "trianglesurface.h"
#include<iostream>

TriangleSurface::TriangleSurface()
{

    Vertex v0{0.0,0.0,0.0, 1,0,0, 1,1};    mVertices.push_back(v0);
    Vertex v1(0.5,0.0,0.0, 0,1,0, 1,0);    mVertices.push_back(v1);
    Vertex v2{0.5,0.5,0.0, 0,0,1,0,0};    mVertices.push_back(v2);
    Vertex v3{0.0,0.0,0.0, 0,0,1, 1,0};    mVertices.push_back(v3);
    Vertex v4{0.5,0.5,0.0, 0,1,0,0,1};    mVertices.push_back(v4);
    Vertex v5{0.0,0.5,0.0, 1,0,0,0,0};    mVertices.push_back(v5);

    mMatrix.setToIdentity();
}

TriangleSurface::TriangleSurface(std::string filnavn)
{
    readFile(filnavn);
    mMatrix.setToIdentity();
}

TriangleSurface::~TriangleSurface()
{

}

void TriangleSurface::readFile(std::string filnavn)
{
    std::ifstream inn;
     inn.open(filnavn.c_str());


      if (inn.is_open()) {
          int n;
          Vertex vertex;
          inn >> n;
          mVertices.reserve(n);
          for (int i=0; i<n; i++) {
               inn >> vertex;
               mVertices.push_back(vertex);
          }
          inn.close();
      }
}

// Parameternavnet er byttet ut fra leksjonen
// Koden er ellers som for XYZ::init()
void TriangleSurface::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width{};
    int height{};
    int nrChannels{};
    unsigned char *data = stbi_load("woodTexture.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


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

    // 3rd texture coord attribute buffer:
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)));
      glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}
