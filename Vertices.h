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
	float vertices[601][601];	//table for all calculated points
	float minmax[3][2];	//table for triangle vertices positions
	float middleZ;	//value of Z coordinate of point which is on X posisition of middle vertex and it's Z position is on straight line which is passing through first and last vertex
	float Zmin;	//maximum Z value of 3 vertices of triangle
	float Zmax;	//minimum Z value of 3 vertices of triangle
	struct vertex
	{
		float x;
		float y;
		float z;
	};
	Vertices(string filename);//Read function
	void IsInside(float x1, float x2, float x3, float z1, float z2, float z3, float y1, float y2, float y3);	//finds points inside the given triangle
	void min(float x1, float x2, float x3, float z1, float z2, float z3);	//finds for a minimum X position value from 3 vertices of triangle
	void max(float x1, float x2, float x3, float z1, float z2, float z3);	//finds maximum X position value from 3 vertices of triangle
	void middle(float x1, float x2, float x3, float z1, float z2, float z3);	//finds middle X position value from 3 vertices of triangle
private:
	vector<vector<GLfloat>> v;//Store vertex (x,y,z) coordinates
	vector<vector<GLint>> f;//Store the three vertex indexes of the face
};
