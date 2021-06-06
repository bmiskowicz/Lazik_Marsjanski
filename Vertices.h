#pragma once
#include <vector>
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include "includes/glut.h"
#include "includes/GL.H"

using namespace std;

class Vertices
{
public:
	float vertices[600][600];
	struct vertex
	{
		float x;
		float y;
		float z;
	};
	Vertices(string filename);//Read function
	void IsInside(int x1, int x2, int x3, int z1, int z2, int z3, int y1, int y2, int y3);
	bool Obszar(int x1, int z1, int x2, int z2, int x3, int z3);
	float min(float v1, float v2, float v3);
	float max(float v1, float v2, float v3);
private:
	vector<vector<GLfloat>> v;//Store vertex (x,y,z) coordinates
	vector<vector<GLint>> f;//Store the three vertex indexes of the face
};