// 3D-programmering 2022
#include "vertex.h"
#include <fstream>
Vertex::Vertex()
{


}
Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    m_xyz[0] = x; m_xyz[1] = y; m_xyz[2] = z;
    m_normal[0] = r; m_normal[1] = g; m_normal[2] = b;
    m_st[0] = 0.0; m_st[1] = 1.0;
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t)
{
    m_xyz[0] = x; m_xyz[1] = y; m_xyz[2] = z;
    m_normal[0] = r; m_normal[1] = g; m_normal[2] = b;
    m_st[0] =s; m_st[1] = t;
}

void Vertex::ModifyVertexColor(float r, float g, float b)
{
    m_normal[0] = r; m_normal[1] = g; m_normal[2] = b;
}

QVector3D Vertex::getXYZ()
{
    QVector3D XYZ={this->m_xyz[0],this->m_xyz[1],this->m_xyz[2]};
    return XYZ;
}

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
  os << "(" << v.m_st[0] << ", " << v.m_st[1] << ") ";
  return os;
}
std::istream& operator>> (std::istream& is, Vertex& v) {
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
  is >> dum >> v.m_st[0] >> dum2 >> v.m_st[1] >> dum3;
  return is;
}
