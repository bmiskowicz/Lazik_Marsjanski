#include <stdlib.h>
#include "includes/glut.h"
#include "includes/GL.H"
#include <cmath>
#include "interpreter.h"
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f, ly = 0.0f;
// XZ position of the camera
float x = 12.0f, z = 10.0f, y = 8.0f;
#define GL_PI 3.14
static GLuint textureName;
int width, height, nrChannels;

Interpreter textures = Interpreter("tekstury.obj");
Interpreter cube = Interpreter("cube.obj");
Interpreter monkey = Interpreter("monkey.obj");


void init(unsigned char* data)
{
	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}

void changeSize(int w, int h) 
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(90.0f, ratio, 0.1f, 1000.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT:	//obrót w lewo
		angle -= 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:	//obrót w prawo
		angle += 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:	//ruch do przodu
		y += ly;
		x += lx;
		z += lz;
		break;
	case GLUT_KEY_DOWN:	//ruch do ty³u
		y -= ly;
		x -= lx;
		z -= lz;
		break;
	case GLUT_KEY_F1:	//obrót w górê
		ly +=0.1f;
		break;
	case GLUT_KEY_F2:	//obrót w dó³
		ly -= 0.1f;
		break;
	}
}

void cuboid(float x, float y, float z, float length, float height, float width, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x + length, y + height, z);
	glVertex3f(x + length, y, z);
	glVertex3f(x + length, y, z + width);
	glVertex3f(x, y, z + width);
	glVertex3f(x, y + height, z + width);
	glVertex3f(x, y + height, z);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x + length, y + height, z + width);
	glVertex3f(x + length, y + height, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x, y + height, z + width);
	glVertex3f(x, y, z + width);
	glVertex3f(x + length, y, z + width);
	glVertex3f(x + length, y, z);
	glVertex3f(x + length, y + height, z);
	glEnd();


	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_LINE_STRIP);
	glVertex3f(x, y + height, z + width);
	glVertex3f(x, y, z + width);
	glVertex3f(x, y, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x + length, y + height, z);
	glVertex3f(x + length, y, z);
	glVertex3f(x + length, y, z + width);
	glVertex3f(x + length, y + height, z + width);
	glVertex3f(x, y + height, z + width);
	glVertex3f(x, y + height, z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(x, y, z);
	glVertex3f(x + length, y, z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(x, y, z + width);
	glVertex3f(x + length, y, z + width);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(x + length, y + height, z);
	glVertex3f(x + length, y + height, z + width);
	glEnd();
}

void cylinder(float R, float x, float y, float z, float length, float red, float green, float blue, int base, int mode)
{
	float alpha;	//k¹t
	glColor3f(red, green, blue);
	if (mode == 0)
	{
		glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - dolnej podstawy
		glVertex3f(x, y, z);	//koordynaty œrodka dolnej podstawy
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
		{
			glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);		//kolejne krawêdzie zewnêtrzne podstawy
		}
		glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);	//domkniêcie podstawy
		glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

		glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
		glVertex3f(x, y, z + length);	//koordynaty œrodka górnej podstawy
		for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
		{
			glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length);		//kolejne krawêdzie zewnêtrzne podstawy
		}
		glVertex3f(x + R * cos(0.0), y + R * sin(0.0), z + length);	//domkniêcie podstawy
		glEnd();	//koniec sklejanego prymitywu - górnej podstawy

		glBegin(GL_TRIANGLE_STRIP);	//pocz¹tki prymitywów 
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolejnych trójk¹tów sk³¹daj¹cych siê w prymityw
		{
			glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);	//koordynaty wierzcho³ków dolnych
			glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length);	//koordynaty wierzcho³ków górnych
		}
		//ostatnia linia prymitywów, jak wy¿ej
		glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);
		glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z + length);
		glEnd();	//zakoñczenie prymitywu
	}
	else if (mode == 1)
	{
		glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - dolnej podstawy
		glVertex3f(x, z, y);	//koordynaty œrodka dolnej podstawy
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
		{
			glVertex3f(x + R * cos(alpha), z, y + R * sin(alpha));		//kolejne krawêdzie zewnêtrzne podstawy
		}
		glVertex3f(x + R * cos(2 * GL_PI), z, y + R * sin(2 * GL_PI));	//domkniêcie podstawy
		glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

		glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
		glVertex3f(x, z + length, y);	//koordynaty œrodka górnej podstawy
		for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
		{
			glVertex3f(x + R * cos(alpha), z + length, y + R * sin(alpha));		//kolejne krawêdzie zewnêtrzne podstawy
		}
		glVertex3f(x + R * cos(0.0), z + length, y + R * sin(0.0));	//domkniêcie podstawy
		glEnd();	//koniec sklejanego prymitywu - górnej podstawy

		glBegin(GL_TRIANGLE_STRIP);	//pocz¹tki prymitywów 
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolejnych trójk¹tów sk³¹daj¹cych siê w prymityw
		{
			glVertex3f(x + R * cos(alpha), z, y + R * sin(alpha));	//koordynaty wierzcho³ków dolnych
			glVertex3f(x + R * cos(alpha), z + length, y + R * sin(alpha));	//koordynaty wierzcho³ków górnych
		}
		//ostatnia linia prymitywów, jak wy¿ej
		glVertex3f(x + R * cos(2 * GL_PI), z, y + R * sin(2 * GL_PI));
		glVertex3f(x + R * cos(2 * GL_PI), z + length, y + R * sin(2 * GL_PI));
		glEnd();	//zakoñczenie prymitywu
	}
	else if (mode == 2)
	{
		glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - dolnej podstawy
		glVertex3f(z, y, x);	//koordynaty œrodka dolnej podstawy
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
		{
			glVertex3f(z, y + R * sin(alpha), x + R * cos(alpha));		//kolejne krawêdzie zewnêtrzne podstawy
		}
		glVertex3f(z, y + R * sin(2 * GL_PI), x + R * cos(2 * GL_PI));	//domkniêcie podstawy
		glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

		glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
		glVertex3f(z + length, y, x);	//koordynaty œrodka górnej podstawy
		for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
		{
			glVertex3f(z + length, y + R * sin(alpha), x + R * cos(alpha));		//kolejne krawêdzie zewnêtrzne podstawy
		}
		glVertex3f(z + length, y + R * sin(0.0), x + R * cos(0.0));	//domkniêcie podstawy
		glEnd();	//koniec sklejanego prymitywu - górnej podstawy

		glBegin(GL_TRIANGLE_STRIP);	//pocz¹tki prymitywów 
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolejnych trójk¹tów sk³¹daj¹cych siê w prymityw
		{
			glVertex3f(z, y + R * sin(alpha), x + R * cos(alpha));	//koordynaty wierzcho³ków dolnych
			glVertex3f(z + length, y + R * sin(alpha), x + R * cos(alpha));	//koordynaty wierzcho³ków górnych
		}
		//ostatnia linia prymitywów, jak wy¿ej
		glVertex3f(z, y + R * sin(2 * GL_PI), x + R * cos(2 * GL_PI));
		glVertex3f(z + length, y + R * sin(2 * GL_PI), x + R * cos(2 * GL_PI));
		glEnd();	//zakoñczenie prymitywu
	}
}

void axle(float R, float x, float y, float z, float length, float height, float red, float green, float blue, int base)
{
	float alpha;	//k¹t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - dolnej podstawy
	glVertex3f(x, y, z);	//koordynaty œrodka dolnej podstawy
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - dolnej podstawy
	
	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
	glVertex3f(x, y - height, z + length);	//koordynaty œrodka górnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha) - height, z + length);		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(x + R * cos(0.0), y + R * sin(0.0) - height, z + length);	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - górnej podstawy

	glBegin(GL_TRIANGLE_STRIP);	//pocz¹tki prymitywów 
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolejnych trójk¹tów sk³¹daj¹cych siê w prymityw
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);	//koordynaty wierzcho³ków dolnych
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha) - height, z + length);	//koordynaty wierzcho³ków górnych
	}
	//ostatnia linia prymitywów, jak wy¿ej
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI) - height, z + length);
	glEnd();	//zakoñczenie prymitywu
}

void wheel(float R, float x, float y, float z, float length, float red, float green, float blue, int base)
{
	float alpha;	//k¹t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - dolnej podstawy
	glVertex3f(x, y, z);	//koordynaty œrodka dolnej podstawy
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - dolnej podstawy
	
	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
	glVertex3f(x, y, z + length);	//koordynaty œrodka górnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length);		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(x + R * cos(0.0), y + R * sin(0.0), z + length);	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - górnej podstawy
	
	glBegin(GL_TRIANGLE_STRIP);	//pocz¹tki prymitywów 
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolejnych trójk¹tów sk³¹daj¹cych siê w prymityw
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);	//koordynaty wierzcho³ków dolnych
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length);	//koordynaty wierzcho³ków górnych
	}
	//ostatnia linia prymitywów, jak wy¿ej
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z + length);
	glEnd();	//zakoñczenie prymitywu

	glColor3f(red+0.1, green+0.1f, blue+0.1f);
	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - dolnej podstawy
	glVertex3f(x, y, z - 0.01f);	//koordynaty œrodka dolnej podstawy
	R *= 0.6f;
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z - 0.01f);		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z - 0.01f);	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
	glVertex3f(x, y, z + length + 0.01f);	//koordynaty œrodka górnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length + 0.01f);		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(x + R * cos(0.0), y + R * sin(0.0), z + length + 0.01f);	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - górnej podstawy
}

void cone(float R, float x, float y, float z, float height, float red, float green, float blue, int base)
{
	float alpha;	//k¹t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
	glVertex3f(x, z , y);	//koordynaty œrodka górnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(x + R * cos(alpha), z , y + R * sin(alpha));		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(x + R * cos(0.0), z , y + R * sin(0.0));	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - górnej podstawy
	
	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
	glVertex3f(x, z + height, y);	//koordynaty œrodka górnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(x + R * cos(alpha), z, y + R * sin(alpha));		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(x + R * cos(0.0), z, y + R * sin(0.0));	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - górnej podstawy
}

void antenna(float R, float x, float y, float z, float height, float red, float green, float blue, int base)
{
	float alpha;	//k¹t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz¹tek sklejanego prymitywu - górnej podstawy
	glVertex3f(z, y, x);	//koordynaty œrodka górnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//pêtla dla kolenych trójk¹tów tworz¹cych podstawê
	{
		glVertex3f(z + height, y + R * sin(alpha), x + R * cos(alpha));		//kolejne krawêdzie zewnêtrzne podstawy
	}
	glVertex3f(z+ height, y + R * sin(0.0), x + R * cos(0.0));	//domkniêcie podstawy
	glEnd();	//koniec sklejanego prymitywu - górnej podstawy
}

void renderScene(void) {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	// Reset transformations
	glLoadIdentity();
	// Set the camera

	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);



	cuboid(0.0f, 5.0f, 0.0f, 10.0f, 2.0f, 5.0f, 0.5, 0.5, 0.5);
	cylinder(0.3f, 1.0f, 4.7f, 0.0f, 5.0f, 0.3, 0.3, 0.3, 20, 0);
	cylinder(0.3f, 5.0f, 4.7f, 0.0f, 5.0f, 0.3, 0.3, 0.3, 20, 0);
	cylinder(0.3f, 9.0f, 4.7f, 0.0f, 5.0f, 0.3, 0.3, 0.3, 20, 0);


	axle(0.3f, 1.0f, 4.7f, 5.0f, 1.0f, 1.5f, 0.2, 0.2, 0.2, 20);
	axle(0.3f, 5.0f, 4.7f, 5.0f, 1.0f, 1.5f, 0.2, 0.2, 0.2, 20);
	axle(0.3f, 9.0f, 4.7f, 5.0f, 1.0f, 1.5f, 0.2, 0.2, 0.2, 20);

	wheel(1.0f, 1.0f, 3.2f, 6.0f, 1.0f, 0.0, 0.0, 0.0, 5);
	wheel(1.0f, 5.0f, 3.2f, 6.0f, 1.0f, 0.0, 0.0, 0.0, 5);
	wheel(1.0f, 9.0f, 3.2f, 6.0f, 1.0f, 0.0, 0.0, 0.0, 5);


	axle(0.3f, 0.9f, 4.7f, 0.0f, -1.0f, 1.5f, 0.2, 0.2, 0.2, 20);
	axle(0.3f, 4.9f, 4.7f, 0.0f, -1.0f, 1.5f, 0.2, 0.2, 0.2, 20);
	axle(0.3f, 8.9f, 4.7f, 0.0f, -1.0f, 1.5f, 0.2, 0.2, 0.2, 20);

	wheel(1.0f, 0.9f, 3.2f, -2.0f, 1.0f, 0.0, 0.0, 0.0, 5);
	wheel(1.0f, 4.9f, 3.2f, -2.0f, 1.0f, 0.0, 0.0, 0.0, 5);
	wheel(1.0f, 8.9f, 3.2f, -2.0f, 1.0f, 0.0, 0.0, 0.0, 5);


	cylinder(0.1f, 3.0f, 3.5f, 7.0f, 2.0f, 0.3, 0.3, 0.3, 20, 1);
	cylinder(0.1f, 3.0f, 3.5f, 9.0f, 0.2f, 0.9, 0.0, 0.0, 20, 1);


	cylinder(0.1f, 9.0f, 0.5f, 7.0f, 2.0f, 0.3, 0.3, 0.3, 20, 1);
	cone(0.1f, 9.0f, 0.5f, 9.0f, 0.2f, 0.3, 0.3, 0.3, 20);
	antenna(0.7f, 0.5f, 9.2f, 9.0f, 0.5f, 0.2, 0.2, 0.2, 20);

	cylinder(0.1f, 9.0f, 4.5f, 7.0f, 2.0f, 0.3, 0.3, 0.3, 20, 1);
	cone(0.1f, 9.0f, 4.5f, 9.0f, 0.2f, 0.3, 0.3, 0.3, 20);
	antenna(0.7f, 4.5f, 9.2f, 9.0f, 0.5f, 0.2, 0.2, 0.2, 20);


	cylinder(0.1f, 9.0f, 2.5f, 7.0f, 0.5f, 0.3, 0.3, 0.3, 20, 1);
	cuboid(8.5f, 7.5f, 2.25f, 1.5f, 0.5f, 0.5f, 0.5, 0.5, 0.5);
	cylinder(0.1f, 2.5f, 7.75f, 10.0f, 0.2f, 0.1, 0.1, 0.1, 20, 2);

	glColor3f(1.0, 1.0, 1.0);
	init(stbi_load("texture.jpg", &width, &height, &nrChannels, 0));
	textures.DrawT();
	glDeleteTextures(1, &textureName);

	init(stbi_load("monkey.jpg", &width, &height, &nrChannels, 0));
	monkey.DrawT();
	glDeleteTextures(1, &textureName);

	glColor3f(0, 0, 0);
	cube.Draw();
	
	glutSwapBuffers();
}


int main(int argc, char** argv) 
{

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1080, 860);
	glutCreateWindow("£azik Marsjañski");
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	// here are the new entries
	glutSpecialFunc(processSpecialKeys);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	// enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}