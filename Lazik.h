#pragma once
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include "includes/glut.h"
#include "includes/GL.H"

using namespace std;

class Lazik
{
public:
	Lazik();
	void cuboid(float x, float y, float z, float length, float height, float width, float red, float green, float blue);
	void cylinder(float R, float x, float y, float z, float length, float red, float green, float blue, int base, int mode);
	void axle(float R, float x, float y, float z, float length, float height, float red, float green, float blue, int base);
	void wheel(float R, float x, float y, float z, float length, float red, float green, float blue, int base);
	void cone(float R, float x, float y, float z, float height, float red, float green, float blue, int base);
	void antenna(float R, float x, float y, float z, float height, float red, float green, float blue, int base);
	void wheel1();
	void wheel2();
};