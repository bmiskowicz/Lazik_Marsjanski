#include <stdlib.h>
#include "includes/glut.h"
#include "includes/GL.H"
#include <cmath>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "interpreter.h"
#include "Lazik.h"

// angle of rotation for the camera direction
float camera_angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f, ly = 0.0f;
// XZ position of the camera
float x = 12.0f, z = 10.0f, y = 15.0f;
#define GL_PI 3.14
static GLuint textureName;
int width, height, nrChannels;
//objects from blender
Interpreter textures = Interpreter("tekstury.obj");
Interpreter cube = Interpreter("cube.obj");
Interpreter icosphere = Interpreter("Icosphere.obj");
//coordinates of rover
float pos_x = 0;
float pos_z = 0;
float phrase = 0.2; 
float angle = 1.6;
float act_angle = 1.6;
float rotation = 0;
float move_rotation = 0;
float old_rotation = 0;

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
	case GLUT_KEY_LEFT:	//obr�t w lewo
		camera_angle -= 0.1f;
		lx = sin(camera_angle);
		lz = -cos(camera_angle);
		break;
	case GLUT_KEY_RIGHT:	//obr�t w prawo
		camera_angle += 0.1f;
		lx = sin(camera_angle);
		lz = -cos(camera_angle);
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

void processKeyboardKeys(unsigned char key, int x, int y)
{
	cout << angle << endl;
	switch (key) {
	case 'w':
		move_rotation = rotation;
		if (phrase < 0.7)
		{
			phrase = phrase + 0.05;
		}			
		if (angle == 0)	pos_x += phrase;
		else
		{
			pos_x += sin(angle) * phrase;
			pos_z += -cos(angle) * phrase;
		}
		break;
	case 's':
		move_rotation = rotation;
		if (phrase < 0.7)
		{
			phrase = phrase + 0.05;
		}
		if (angle == 0)	pos_x -= phrase;
		else
		{
			pos_x -= sin(angle) * phrase;
			pos_z -= -cos(angle) * phrase;
		}
		break;
	case 'd':
		if (angle < 4.76)
		{
			angle += 0.05;
			rotation = (360 - 0) * (-angle + 1.6) / (4.8 + 1.6) + 0;
		}
		else if (angle > 4.76)
		{
			angle = -1.6;
			rotation = (360 - 0) * (-angle + 1.6) / (4.8 + 1.6) + 0;
		}
		break;
	case 'a':
		if (angle > -1.56)
		{
			angle -= 0.05;
			rotation = (360 - 0) * (-angle + 1.6) / (4.8 + 1.6) + 0;
		}
		else if (angle < -1.56)
		{
			angle = 4.8;
			rotation = (360 - 0) * (-angle + 1.6) / (4.8 + 1.6) + 0;
		}
		break;
	}
}

void renderScene(void) {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

	glColor3f(1.0, 1.0, 1.0);
	init(stbi_load("textures.jpg", &width, &height, &nrChannels, 0));
	textures.DrawT();
	glDeleteTextures(1, &textureName);

	init(stbi_load("stone.jpg", &width, &height, &nrChannels, 0));
	icosphere.DrawT();
	glDeleteTextures(1, &textureName);

	glColor3f(0, 0, 0);
	cube.Draw();


	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(pos_x, 0.0, pos_z);
	glRotatef(move_rotation, 0, 1, 0);
	glTranslatef(0.0, 0.0, 0.0);
	Lazik Marsjanski = Lazik();
	glPopMatrix();



	if (rotation == move_rotation)
	{
		glPushMatrix();
		glTranslatef(pos_x, 0.0, pos_z);
		glRotatef(move_rotation, 0, 1, 0);
		glTranslatef(0.0, 0.0, 0.0);
		Marsjanski.wheel1();
		glPopMatrix();


		glPushMatrix();
		glTranslatef(pos_x, 0.0, pos_z);
		glRotatef(move_rotation, 0, 1, 0);
		glTranslatef(0.0, 0.0, 0.0);
		Marsjanski.wheel2();
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glTranslatef(pos_x, 0.0, pos_z);
		glRotatef(move_rotation, 0, 1, 0);
		glTranslatef(0.0, 0.0, 0.0);

		glTranslatef(9.0, 0.0, 3.5);
		glRotatef(rotation, 0, 1, 0);
		glTranslatef(-9.0, 0.0, -3.5);
		Marsjanski.wheel1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(pos_x, 0.0, pos_z);
		glRotatef(move_rotation, 0, 1, 0);
		glTranslatef(0.0, 0.0, 0.0);

		glTranslatef(9.0f, 0.0, -4.5f);
		glRotatef(rotation, 0, 1, 0);
		glTranslatef(-9.0f, 0.0, 4.5f);
		Marsjanski.wheel2();
		glPopMatrix();
	}



	//gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	glutSwapBuffers();
}

int main(int argc, char** argv) 
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1080, 860);
	glutCreateWindow("�azik Marsja�ski");
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	// here are the new entries
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processKeyboardKeys);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	// enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}