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
	float minmax[3][2];
	float middleZ;
	float Zmin;
	float Zmax;
	struct vertex
	{
		float x;
		float y;
		float z;
	};
	Vertices(string filename);//Read function
	void IsInside(float x1, float x2, float x3, float z1, float z2, float z3, float y1, float y2, float y3);
	void min(float x1, float x2, float x3, float z1, float z2, float z3);
	void max(float x1, float x2, float x3, float z1, float z2, float z3);
	void middle(float x1, float x2, float x3, float z1, float z2, float z3);
private:
	vector<vector<GLfloat>> v;//Store vertex (x,y,z) coordinates
	vector<vector<GLint>> f;//Store the three vertex indexes of the face
};
