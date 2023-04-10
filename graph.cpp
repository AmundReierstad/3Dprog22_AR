#include "graph.h"
#include<math.h>
Graph::Graph()
{

}

Graph::Graph(bool isTask4_4_4)
{
    if (isTask4_4_4)
    {
        float x=-1.f;
        for (int i=0;i<20;i++)
        {
            x+=2/20.f;
            double y;
            y= pow(x,2);
            mVertices.push_back(Vertex(x,y,0,1,1,1));
        }
        return;
    }

    QMatrix4x4 a = QMatrix4x4(0.f,0.f,0.f,1.f,
                              1.f,1.f,1.f,1.f,
                              8.f,4.f,2.f,1.f,
                              27.f,9.f,3.f,1.f);

    QMatrix4x4 aInv{};
    aInv=a.inverted();

    QVector4D Y=QVector4D(0,-1,1,2);

    QVector4D X;

    X=aInv*Y;
    // contains coefficents to 3d grade polynom

    //we now construct with res 100 over 0 to 3
    double x{0};
    double y{};
    for (int i=0;i<100;i++)
    {

        y= (pow(x*X.x(),3)+pow(x*X.y(),2)+x*X.z()+X.w());
        mVertices.push_back(Vertex(x,y,0,0,1,1));
        x=x+0.03;
    }
}

Graph::~Graph()
{

}

Graph::Graph(std::string filnavn)
{
    readFile(filnavn);
    mMatrix.setToIdentity();
}

void Graph::readFile(std::string filnavn)
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

void Graph::init(GLint matrixUniform)
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

void Graph::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
}
