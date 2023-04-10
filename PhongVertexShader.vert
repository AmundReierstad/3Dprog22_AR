#version 410 core

layout(location = 0) in vec3 vertexPosition;   // 1st attribute buffer = vertex positions
layout(location = 1) in vec3 vertexNormal;   // 2nd attribute buffer = vertex normals, for light calculation
layout (location = 2) in vec2 vertexUV; //3rd attribute buffer, vertex UV position

out vec3 fragmentPosition;
out vec3 normalTransposed;
//out vec2 textureCoordinates;      //texture UV
uniform mat4 pMatrix;                   // projection
uniform mat4 vMatrix;                   // view
uniform mat4 mMatrix;                 // transform


void main()
{
  fragmentPosition = vec3(mMatrix*vec4(vertexPosition,1.0));               //passing on the vertex color, 1 because it is a point
  normalTransposed=mat3(transpose(inverse(mMatrix)))* vertexNormal; //multiply with transposed inverse of mMatrix, to counteract skewed normals upon non-uniform scaling
  gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertexPosition,1.0);      //calculate the position of the model
   //textureCoordinates=textureIn;
}
