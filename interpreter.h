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
	void DrawT();//Drawing function for objects with textures
	void Draw();//Drawing function
	void DrawObj(float posX, float posY, float posZ);//Star drawing function
private:
	vector<vector<GLfloat>> v;//Store vertex (x,y,z) coordinates
	vector<vector<GLint>> f;//Store the three vertex indexes of the face
};