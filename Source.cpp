#include <stdlib.h>
#include "includes/glut.h"
#include "includes/GL.H"
#include <cmath>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "interpreter.h"
#include "Lazik.h"
#include "Vertices.h"

#define GL_PI 3.14

// angle of rotation for the camera direction
float camera_angle = 1.5;
// actual vector representing the camera's direction
float lx = 1.0f, lz = 0.0f, ly = -0.5f;
// XZ position of the camera
float x = 0.0f, z = 0.0f, y = 15.0f;
static GLuint textureName;
int width, height, nrChannels;
//objects from blender
Interpreter textures = Interpreter("tekstury.obj");
Interpreter cube = Interpreter("cube.obj");
Interpreter icosphere = Interpreter("Icosphere.obj");
Vertices coordinates = Vertices("tekstury.obj");

//rover's x and y coordinates
float pos_x = 0;
float pos_z = 0;

//rover's rotation variables
float turning_angle = 0.0;
float rover_angle = 0.0;
float rotation = 0.0;

//rover velocities
float speed = 0.0;
float angular_speed = 0.0;

//antennas' rotation variable
float antenna_rotation = 0.0;
float antenna_angle = 0.0;

//variable for diode
int diode_time = 0;;

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
	case GLUT_KEY_LEFT:	//turn left
		camera_angle -= 0.1f;
		lx = sin(camera_angle);
		lz = -cos(camera_angle);
		break;
	case GLUT_KEY_RIGHT:	//turn right
		camera_angle += 0.1f;
		cout << camera_angle << endl;
		lx = sin(camera_angle);
		lz = -cos(camera_angle);
		break;
	case GLUT_KEY_UP:	//move forward
		y += ly;
		x += lx;
		z += lz;
		break;
	case GLUT_KEY_DOWN:	//move backward
		y -= ly;
		x -= lx;
		z -= lz;
		break;
	case GLUT_KEY_F1:	//turn up
		ly +=0.1f;
		break;
	case GLUT_KEY_F2:	//turn down
		ly -= 0.1f;
		break;
	}
}

void processKeyboardKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		if (speed < 1.5)	speed += 0.03;	// increasing speed till limit when moving forward, or decreasing speed when moving backwards
		if(turning_angle == 0)	//when turning angle is 0
		{
			rotation = 0;	//rover is not rotating
			angular_speed = 0;	//and agular speed is none
		}
		break;
	case 's':
		if (speed > -0.6)	speed -= 0.03;	//decreasing speed till limit when moveing forward, or increasing speed when moving backwards
		if (turning_angle == 0)	//when turning angle is 0
		{
			rotation = 0;	//rover is not rotating
			angular_speed = 0;	//and agular speed is none
		}
		break;
	case 'd':
			turning_angle += 8;	//increasing turning angle by 8 degrees
			rotation = 0.8 / tan(turning_angle / (180 / GL_PI));	//calculating the rotation of rover
			angular_speed = -speed / rotation;	//calculating the angular_speed
		break;
	case 'a':
			turning_angle -= 8;	//decreasing turning angle by 8 degrees
			rotation = 0.8 / tan(turning_angle / (180 / GL_PI));	//calculating the rotation of rover
			angular_speed = -speed / rotation;	//calculating the angular_speed
		break;
	}
}

void timerCallback(int value)
{
	glutTimerFunc(100, timerCallback, 0);	//called every 100ms
	speed = 0.95 * speed;	//decreasing the speed by 5%
	diode_time += 1;	//iteration the diode glowing variable
	if (diode_time > 14)	diode_time = 0;		//every 1500ms reseting it to 0
}

void renderScene(void) {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);



	init(stbi_load("stone.jpg", &width, &height, &nrChannels, 0));
	icosphere.DrawT();
	glDeleteTextures(1, &textureName);

	glColor3f(0, 0, 0);
	cube.Draw();



	for (int i = 0; i <= 600; i++)
	{
		for (int j = 0; j <= 600; j++)
		{
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_POINTS);
			glVertex3f(i - 300, coordinates.vertices[i][j], j - 300);
			glEnd();
		}
		cout << endl;
	}



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
	//OpenGL init
	glEnable(GL_DEPTH_TEST);
	//Glut timer
	glutTimerFunc(100, timerCallback, 0);
	// enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}