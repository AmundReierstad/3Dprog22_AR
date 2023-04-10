#ifndef CAMERA_H
#define CAMERA_H
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include<QVector3D>

class Camera: public QOpenGLFunctions_4_1_Core
{
private:
    //shader uniforms
   GLint  mPmatrixUniform;        //OpenGL reference to the Uniform in the shader program
   GLint  mVmatrixUniform;        //OpenGL reference to the Uniform in the shader program
   GLint  mPmatrixUniformPhong;        //OpenGL reference to the Uniform in the phong shader program
   GLint  mVmatrixUniformPhong;        //OpenGL reference to the Uniform in the phong shader program
   //matrixes:
   QMatrix4x4 mPmatrix{};         // denne,
   QMatrix4x4 mVmatrix{};         // og denne, skal legges inn i kameraklasse
public:
   Camera();
   ~Camera() { }
   QVector3D mPosition; //position
   QVector3D mUpDirection;
   QVector3D mFrontDirection;

   float cameraSpeed;

   void init(GLint pMatrixUniform, GLint vMatrixUniform,GLint pMatrixUniformPhong, GLint vMatrixUniformPhong);
   void perspective(int degrees, double aspect, double nearplane, double farplane);
   void lookAt(const QVector3D& mPosition, const QVector3D& object, const QVector3D& up);
   void update(std::string activeShader);
   void translate(float dx, float dy, float dz);
};

#endif // CAMERA_H
