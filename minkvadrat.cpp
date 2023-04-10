#include "minkvadrat.h"

#include "graph.h"

MinKvadrat::MinKvadrat()
{
 // punkter a0 (1,1), a1(2,2), a2(3,3), a3(4,4), a4(5,3), a5(6,2), a6(7,1)

    const int A_Trow{2};
    const int A_Tcol{7};
    const int Arow{7};
    const int Acol{2};
    const int Yn{7};
    double A_T [A_Trow][A_Tcol]= {
                        {1,2,3,4,5,6,7},
                        {1,2,3,4,3,2,1}
                       };
    double A [Arow][Acol]=
    {
        {1,2},
        {1,2},
        {3,3},

        {4,4},
        {5,3},
        {6,2},
        {7,1}
    };
    double Y[Yn]=
    {
        1,
        2,
        3,
        4,
        3,
        2,
        1
    };
    //--------------------Create, init and calculate resultant matrixes----------------
    // create and init resultant matrix forA_TxA with shape ATrow,Acol
    double A_TxA[A_Trow][Acol];
       for(int i = 0; i < A_Trow; i++){
          for(int j = 0; j < Acol; j++){
              A_TxA[i][j] = 0;
          }
      }
      //  Finding the product of 2 matrices and storing it in resultant matrix
      for(int i = 0; i < A_Trow; i++){
          for(int j = 0; j < Acol; j++){
              for(int k = 0; k < A_Tcol;k++){
                  A_TxA[i][j] += A_T[i][k]*A[k][j];
              }
          }
      }

     // create and init resultant matrix forA_TxY with shape ATrow,Ycol(1)
     double A_TxY[A_Trow][1];
         for(int i = 0; i < A_Trow; i++){
            for(int j = 0; j < 1; j++){
                A_TxA[i][j] = 0;
            }
        }
     //  Finding the product of 2 matrices and storing it in resultant matrix
     for(int i = 0; i < A_Trow; i++){
         for(int j = 0; j < 1; j++){
             for(int k = 0; k < A_Tcol;k++){
                 A_TxY[i][j] += A_T[i][k]*Y[k];
             }
         }
     }
    //find inverse of A_TxA, use formula for 2x2 matrix

    double A_TxAinverse [sizeof(A_TxA)/sizeof(A_TxA[0])][sizeof(A_TxA[0])/sizeof(double)];
    int fac{};
    // calculate for latter use: 1/ad-bc
    fac=(1/(A_TxA[0][0]*A_TxA[1][1]-A_TxA[0][1]*A_TxA[1][0]));
    //set values in accordance to formula
    A_TxAinverse[0][0]=fac*A_TxA[1][1];
    A_TxAinverse[0][1]=fac*-A_TxA[0][1];
    A_TxAinverse[1][0]=fac*-A_TxA[1][0];
    A_TxAinverse[1][1]=fac*-A_TxA[0][0];
    //bruker teorem Bx=c, finner x ved x=B^-1*c
    //A_TxA -> B, A_TxY ->c


    double Xvec[2];
    for(int i = 0; i <2; i++){
        for(int j = 0; j < 1; j++){
            for(int k = 0; k < 2;k++){
                Xvec[i] += A_TxAinverse[i][k]*A_TxY[k][j];
            }
        }
    }
}



MinKvadrat::~MinKvadrat()
{

}

MinKvadrat::MinKvadrat(std::string filnavn)
{
    readFile(filnavn);
    mMatrix.setToIdentity();
}

void MinKvadrat::readFile(std::string filnavn)
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

void MinKvadrat::init(GLint matrixUniform)
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

void MinKvadrat::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());
}


