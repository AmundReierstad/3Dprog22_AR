#include "camera.h"

Camera::Camera()
{
    mPosition={0,1,5};
    mUpDirection={0,1,0};
    mFrontDirection={0,0,-1};
    cameraSpeed=0.01f;
}

void Camera::init(GLint pMatrixUniform, GLint vMatrixUniform,GLint pMatrixUniformPhong, GLint vMatrixUniformPhong)
{
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();
    mPmatrixUniform=pMatrixUniform;
    mVmatrixUniform=vMatrixUniform;
    mPmatrixUniformPhong=pMatrixUniformPhong;
    mVmatrixUniformPhong=vMatrixUniformPhong;
}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPmatrix.perspective(degrees,aspect,nearplane,farplane);
}

void Camera::lookAt(const QVector3D &position, const QVector3D &object, const QVector3D &up)
{
    mVmatrix.lookAt(position,object,up);
}

void Camera::update(std::string activeShader)
{
     initializeOpenGLFunctions();
    if (activeShader=="plain")
    {
    glUniformMatrix4fv(mPmatrixUniform,1,GL_FALSE, mPmatrix.constData());
    glUniformMatrix4fv(mVmatrixUniform,1,GL_FALSE, mVmatrix.constData());
    }
    if(activeShader=="phong")
    {
    glUniformMatrix4fv(mPmatrixUniformPhong,1,GL_FALSE, mPmatrix.constData());
    glUniformMatrix4fv(mVmatrixUniformPhong,1,GL_FALSE, mVmatrix.constData());
    }
}

//void Camera::translate(float dx, float dy, float dz)
//{

//}
