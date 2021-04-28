#pragma once
#include <vector>
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include "includes/glut.h"
#include "includes/GL.H"

using namespace std;

class Interpreter
{
public:
	struct vertex
	{
		float x;
		float y;
		float z;
	};
	Interpreter(string filename);//Read function
	void Draw();//Drawing function
private:
	vector<vector<GLfloat>> v;//Store vertex (x,y,z) coordinates
	vector<vector<GLint>> f;//Store the three vertex indexes of the face
};