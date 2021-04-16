#include <stdlib.h>
#include "includes/glew.h"
#include "includes/glut.h"
#include "includes/GL.H"
#include <cmath>

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f, ly = 0.0f;
// XZ position of the camera
float x = 0.0f, z = 0.0f, y = 0.0f;

#define GL_PI 3.14


void changeSize(int w, int h) {

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

void cylinder(float R, float x, float y, float z, float width, float red, float green, float blue)
{
	float alpha;	//k�t
	glColor3f(red, green, blue);

	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - dolnej podstawy
	glVertex3f(x, y, z);	//koordynaty �rodka dolnej podstawy
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / 20)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - dolnej podstawy

	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(x, y, z + width);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / 20)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + width);		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(0.0), y + R * sin(0.0), z + width);	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy

	glBegin(GL_TRIANGLE_STRIP);	//pocz�tki prymityw�w 
	for (alpha = 2 * GL_PI; alpha >= 0; alpha -= GL_PI / 20)	//p�tla dla kolejnych tr�jk�t�w sk��daj�cych si� w prymityw
	{
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z);	//koordynaty wierzcho�k�w dolnych
		glVertex3f(x + R * cos(alpha), y + R * sin(alpha), z + width);	//koordynaty wierzcho�k�w g�rnych
	}
	//ostatnia linia prymityw�w, jak wy�ej
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z);
	glVertex3f(x + R * cos(2 * GL_PI), y + R * sin(2 * GL_PI), z + width);
	glEnd();	//zako�czenie prymitywu
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

void renderScene(void) {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_BACK, GL_LINE);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	


	cuboid(0.0f, 50.0f, 0.0f, 100.0f, 20.0f, 50.0f, 0.5, 0.5, 0.5);
	cylinder(5.0f, 10.0f, 45.0f, -10.0f, 70.0f, 0.2, 0.2, 0.2);
	cylinder(5.0f, 50.0f, 45.0f, -10.0f, 70.0f, 0.2, 0.2, 0.2);
	cylinder(5.0f, 90.0f, 45.0f, -10.0f, 70.0f, 0.2, 0.2, 0.2);


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