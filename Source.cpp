#include <stdlib.h>
#include "includes/glut.h"
#include "includes/GL.H"
#include <cmath>

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f, ly = 0.0f;
// XZ position of the camera
float x = 120.0f, z = 100.0f, y = 80.0f;
#define GL_PI 3.14

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
	case GLUT_KEY_LEFT:	//obr�t w lewo
		angle -= 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:	//obr�t w prawo
		angle += 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:	//ruch do przodu
		y += ly;
		x += lx;
		z += lz;
		break;
	case GLUT_KEY_DOWN:	//ruch do ty�u
		y -= ly;
		x -= lx;
		z -= lz;
		break;
	case GLUT_KEY_F1:	//obr�t w g�r�
		ly +=0.1f;
		break;
	case GLUT_KEY_F2:	//obr�t w d�
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
	float alpha;	//k�t
	glColor3f(red, green, blue);
	if (mode == 0)
	{
		glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - dolnej podstawy
		glVertex3f(x, y, z);	//koordynaty �rodka dolnej podstawy
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
		{
			glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);		//kolejne kraw�dzie zewn�trzne podstawy
		}
		glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);	//domkni�cie podstawy
		glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

		glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
		glVertex3f(x, y, z + length);	//koordynaty �rodka g�rnej podstawy
		for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
		{
			glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length);		//kolejne kraw�dzie zewn�trzne podstawy
		}
		glVertex3f(x + R * cos(0.0), y + R * sin(0.0), z + length);	//domkni�cie podstawy
		glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy

		glBegin(GL_TRIANGLE_STRIP);	//pocz�tki prymityw�w 
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolejnych tr�jk�t�w sk��daj�cych si� w prymityw
		{
			glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);	//koordynaty wierzcho�k�w dolnych
			glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length);	//koordynaty wierzcho�k�w g�rnych
		}
		//ostatnia linia prymityw�w, jak wy�ej
		glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);
		glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z + length);
		glEnd();	//zako�czenie prymitywu
	}
	else if (mode == 1)
	{
		glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - dolnej podstawy
		glVertex3f(x, z, y);	//koordynaty �rodka dolnej podstawy
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
		{
			glVertex3f(x + R * cos(alpha), z, y + R * sin(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
		}
		glVertex3f(x + R * cos(2 * GL_PI), z, y + R * sin(2 * GL_PI));	//domkni�cie podstawy
		glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

		glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
		glVertex3f(x, z + length, y);	//koordynaty �rodka g�rnej podstawy
		for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
		{
			glVertex3f(x + R * cos(alpha), z + length, y + R * sin(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
		}
		glVertex3f(x + R * cos(0.0), z + length, y + R * sin(0.0));	//domkni�cie podstawy
		glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy

		glBegin(GL_TRIANGLE_STRIP);	//pocz�tki prymityw�w 
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolejnych tr�jk�t�w sk��daj�cych si� w prymityw
		{
			glVertex3f(x + R * cos(alpha), z, y + R * sin(alpha));	//koordynaty wierzcho�k�w dolnych
			glVertex3f(x + R * cos(alpha), z + length, y + R * sin(alpha));	//koordynaty wierzcho�k�w g�rnych
		}
		//ostatnia linia prymityw�w, jak wy�ej
		glVertex3f(x + R * cos(2 * GL_PI), z, y + R * sin(2 * GL_PI));
		glVertex3f(x + R * cos(2 * GL_PI), z + length, y + R * sin(2 * GL_PI));
		glEnd();	//zako�czenie prymitywu
	}
	else if (mode == 2)
	{
		glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - dolnej podstawy
		glVertex3f(z, y, x);	//koordynaty �rodka dolnej podstawy
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
		{
			glVertex3f(z, y + R * sin(alpha), x + R * cos(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
		}
		glVertex3f(z, y + R * sin(2 * GL_PI), x + R * cos(2 * GL_PI));	//domkni�cie podstawy
		glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

		glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
		glVertex3f(z + length, y, x);	//koordynaty �rodka g�rnej podstawy
		for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
		{
			glVertex3f(z + length, y + R * sin(alpha), x + R * cos(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
		}
		glVertex3f(z + length, y + R * sin(0.0), x + R * cos(0.0));	//domkni�cie podstawy
		glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy

		glBegin(GL_TRIANGLE_STRIP);	//pocz�tki prymityw�w 
		for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolejnych tr�jk�t�w sk��daj�cych si� w prymityw
		{
			glVertex3f(z, y + R * sin(alpha), x + R * cos(alpha));	//koordynaty wierzcho�k�w dolnych
			glVertex3f(z + length, y + R * sin(alpha), x + R * cos(alpha));	//koordynaty wierzcho�k�w g�rnych
		}
		//ostatnia linia prymityw�w, jak wy�ej
		glVertex3f(z, y + R * sin(2 * GL_PI), x + R * cos(2 * GL_PI));
		glVertex3f(z + length, y + R * sin(2 * GL_PI), x + R * cos(2 * GL_PI));
		glEnd();	//zako�czenie prymitywu
	}
}

void axle(float R, float x, float y, float z, float length, float height, float red, float green, float blue, int base)
{
	float alpha;	//k�t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - dolnej podstawy
	glVertex3f(x, y, z);	//koordynaty �rodka dolnej podstawy
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - dolnej podstawy
	
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(x, y - height, z + length);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha) - height, z + length);		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(0.0), y + R * sin(0.0) - height, z + length);	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy

	glBegin(GL_TRIANGLE_STRIP);	//pocz�tki prymityw�w 
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolejnych tr�jk�t�w sk��daj�cych si� w prymityw
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);	//koordynaty wierzcho�k�w dolnych
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha) - height, z + length);	//koordynaty wierzcho�k�w g�rnych
	}
	//ostatnia linia prymityw�w, jak wy�ej
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI) - height, z + length);
	glEnd();	//zako�czenie prymitywu
}

void wheel(float R, float x, float y, float z, float length, float red, float green, float blue, int base)
{
	float alpha;	//k�t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - dolnej podstawy
	glVertex3f(x, y, z);	//koordynaty �rodka dolnej podstawy
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - dolnej podstawy
	
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(x, y, z + length);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length);		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(0.0), y + R * sin(0.0), z + length);	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy
	
	glBegin(GL_TRIANGLE_STRIP);	//pocz�tki prymityw�w 
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolejnych tr�jk�t�w sk��daj�cych si� w prymityw
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);	//koordynaty wierzcho�k�w dolnych
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length);	//koordynaty wierzcho�k�w g�rnych
	}
	//ostatnia linia prymityw�w, jak wy�ej
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z + length);
	glEnd();	//zako�czenie prymitywu

	glColor3f(red+0.1, green+0.1f, blue+0.1f);
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - dolnej podstawy
	glVertex3f(x, y, z - 0.01f);	//koordynaty �rodka dolnej podstawy
	R *= 0.6f;
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z - 0.01f);		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z - 0.01f);	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(x, y, z + length + 0.01f);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + length + 0.01f);		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(0.0), y + R * sin(0.0), z + length + 0.01f);	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy
}

void cone(float R, float x, float y, float z, float height, float red, float green, float blue, int base)
{
	float alpha;	//k�t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(x, z , y);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), z , y + R * sin(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(0.0), z , y + R * sin(0.0));	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy
	
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(x, z + height, y);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), z, y + R * sin(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(0.0), z, y + R * sin(0.0));	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy
}

void antenna(float R, float x, float y, float z, float height, float red, float green, float blue, int base)
{
	float alpha;	//k�t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(z, y, x);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(z + height, y + R * sin(alpha), x + R * cos(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(z+ height, y + R * sin(0.0), x + R * cos(0.0));	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy
}

void renderScene(void) {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	
	cuboid(0.0f, 50.0f, 0.0f, 100.0f, 20.0f, 50.0f, 0.5 , 0.5, 0.5);
	cylinder(3.0f, 10.0f, 47.0f, 0.0f, 50.0f, 0.3, 0.3, 0.3, 20, 0);
	cylinder(3.0f, 50.0f, 47.0f, 0.0f, 50.0f, 0.3, 0.3, 0.3, 20, 0);
	cylinder(3.0f, 90.0f, 47.0f, 0.0f, 50.0f, 0.3, 0.3, 0.3, 20, 0);


	axle(3.0f, 10.0f, 47.0f, 50.0f, 10.0f, 15.0f, 0.2, 0.2, 0.2, 20);
	axle(3.0f, 50.0f, 47.0f, 50.0f, 10.0f, 15.0f, 0.2, 0.2, 0.2, 20);
	axle(3.0f, 90.0f, 47.0f, 50.0f, 10.0f, 15.0f, 0.2, 0.2, 0.2, 20);

	wheel(10.0f, 10.0f, 32.0f, 60.0f, 10.0f, 0.0, 0.0, 0.0, 5);
	wheel(10.0f, 50.0f, 32.0f, 60.0f, 10.0f, 0.0, 0.0, 0.0, 5);
	wheel(10.0f, 90.0f, 32.0f, 60.0f, 10.0f, 0.0, 0.0, 0.0, 5);


	axle(3.0f, 10.0f, 47.0f, 0.0f, -10.0f, 15.0f, 0.2, 0.2, 0.2, 20);
	axle(3.0f, 50.0f, 47.0f, 0.0f, -10.0f, 15.0f, 0.2, 0.2, 0.2, 20);
	axle(3.0f, 90.0f, 47.0f, 0.0f, -10.0f, 15.0f, 0.2, 0.2, 0.2, 20);

	wheel(10.0f, 10.0f, 32.0f, -20.0f, 10.0f, 0.0, 0.0, 0.0, 5);
	wheel(10.0f, 50.0f, 32.0f, -20.0f, 10.0f, 0.0, 0.0, 0.0, 5);
	wheel(10.0f, 90.0f, 32.0f, -20.0f, 10.0f, 0.0, 0.0, 0.0, 5);


	cylinder(1.0f, 30.0f, 35.0f, 70.0f, 20.0f, 0.3, 0.3, 0.3, 20, 1);
	cylinder(1.0f, 30.0f, 35.0f, 90.0f, 2.0f, 0.9, 0.0, 0.0, 20, 1);


	cylinder(1.0f, 90.0f, 5.0f, 70.0f, 20.0f, 0.3, 0.3, 0.3, 20, 1);
	cone(1.0f, 90.0f, 5.0f, 90.0f, 2.0f, 0.3, 0.3, 0.3, 20);
	antenna(5.0f, 5.0f, 92.0f, 90.0f, 3.0f, 0.2, 0.2, 0.2, 20);

	cylinder(1.0f, 90.0f, 45.0f, 70.0f, 20.0f, 0.3, 0.3, 0.3, 20, 1);
	cone(1.0f, 90.0f, 45.0f, 90.0f, 2.0f, 0.3, 0.3, 0.3, 20);
	antenna(5.0f, 45.0f, 92.0f, 90.0f, 3.0f, 0.2, 0.2, 0.2, 20);


	cylinder(1.0f, 90.0f, 25.0f, 70.0f, 5.0f, 0.3, 0.3, 0.3, 20, 1);
	cuboid(85.0f, 75.0f, 22.5f, 15.0f, 5.0f, 5.0f, 0.5, 0.5, 0.5);
	cylinder(1.0f, 25.0f, 77.5f, 100.0f, 2.0f, 0.1, 0.1, 0.1, 20, 2);


	glutSwapBuffers();
}


int main(int argc, char** argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1080, 860);
	glutCreateWindow("�azik Marsja�ski");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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