#ifndef GRAPH_H
#define GRAPH_H
#include"visualobject.h"
#include<fstream>

class Graph: public VisualObject
{
public:
    Graph();
    Graph(bool isTask4_4_4);
    ~Graph() override;
    Graph(std::string filnavn);


   void readFile(std::string filnavn);
   void init(GLint matrixUniform) override;
   void draw() override;
};

#endif // GRAPH_H
