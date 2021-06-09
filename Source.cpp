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
#include <string>
#include <iostream>
#define GL_PI 3.14

// Constant definitions for Menus
#define RESTARTMENU 1
#define LVL1 1
#define LVL2 2
#define LVL3 3
#define THIRDPERSON 1
#define DISTANCE 2

// Pop up menu identifiers
int modesTime, modesSpeed, modesCamera, mainMenu, gameModes, easyTime, hardTime, easySpeed, hardSpeed;

// menu status
int menuFlag = 0;

// XZ position of the camera
float x = 0.0f, z = 0.0f, y = 15.0f;
static GLuint textureName;
int width, height, nrChannels;

//objects from blender
Interpreter textures = Interpreter("tekstury.obj");
Interpreter cube = Interpreter("cube.obj");
Interpreter icosphere = Interpreter("Icosphere.obj");

//collisions map
Vertices coordinates = Vertices("tekstury.obj");

//object for collecting points
Interpreter star = Interpreter("gwiazda.obj");
float stars[600][600];	//stars collecting table

//variables for number of stars in level
int min_stars = 3;
int max_stars = 9;

//variables for stars collecting
int numberOfStars = min_stars + (rand() * (int)(max_stars - min_stars) / RAND_MAX);
vector<vector<int> > starsPos(numberOfStars, vector<int>(3));
vector<vector<int> > isStar(numberOfStars, vector<int>(2));

//variables for star random stars coordinates
float randomX = 0.0;
float randomZ = 0.0;


//rover's x and y coordinates
float pos_x = 0;
float pos_z = 0;
float pos_y = 2.2;

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
float diode_time = 0;

//collision points
float backMiddle[] = {0.0, 5.0, 0.0 };
float backWheels[] = { 0.0, 2.2, 0.0 };
float frontWheels[] = { 9.0, 2.2, 0.0 };
float frontMiddle[] = { 10.0, 5.0, 0.0 };

//camera additional variables
float cameraPos[] = { -15.0, 25.0, 0.0 };
float cameraX = 0.0;
float cameraZ = 0.0;

//collision checking variable
bool canMove = 1;

//variable giving different timer height for each camera mode
float timerY = 1.5;

//timer variables 
float timer = 0.0;

float old_timer = 0.0;
string timerstring = to_string(timer);
const char* timerChars = timerstring.c_str();

bool timerFlag = 0;
bool timerMode = 1;
int max_time = 0;

//pause
bool pause = 0;

void processMenuStatus(int status, int x, int y)
{
	if (status == GLUT_MENU_IN_USE)
		menuFlag = 1;
	else
		menuFlag = 0;
}


void processMainMenu(int option)
{
	switch (option)
	{
	case RESTARTMENU:
		turning_angle = 0.0;
		rover_angle = 0.0;
		rotation = 0.0;
		speed = 0.0;
		angular_speed = 0.0;
		pos_x = 0;
		pos_z = 0;
		pos_y = 2.2;

		backMiddle[0] = 0.0;
		backMiddle[1] = 5.0;
		backMiddle[2] = 0.0;
		backWheels[0] = 0.0;
		backWheels[1] = 2.2;
		backWheels[2] = 0.0;
		frontWheels[0] = 9.0;
		frontWheels[1] = 2.2;
		frontWheels[2] = 0.0;
		frontMiddle[0] = 0.0;
		frontMiddle[1] = 5.0;
		frontMiddle[2] = 10.0;
		canMove = 1;

		timer = old_timer;

		for (int i = 0; i < numberOfStars; i++)
		{
			isStar[i][1] = 1;	//creating stars back
		}
		break;
	}
}


void processmodesCamera(int option)
{
	if (abs(timer - max_time) > 0.01)
	{
		switch (option)
		{
		case THIRDPERSON:
			timerY = 1.5;
			cameraX = 0.0;
			cameraZ = 0.0;
			break;
		case DISTANCE:
			timerY = 2.0;
			cameraX = 60.0;
			cameraZ = -0.5;
			break;
		}
	}
}


void processSpeedLevels(int option)
{
	switch (option)
	{
	case LVL1:
		//variables for levels
		numberOfStars = 30;
		timer = 60;
		old_timer = timer;
		break;
	case LVL2:
		numberOfStars = 25;
		timer = 30;
		old_timer = timer;
		break;
	case LVL3:
		numberOfStars = 20;
		timer = 15;
		old_timer = timer;
		break;
	}
	//variables for stars collecting
	starsPos.resize(numberOfStars, vector<int>(3));
	isStar.resize(numberOfStars, vector<int>(2));
	for (int i = 0; i < numberOfStars; ++i)
	{
		randomX = -297 + (rand() / (RAND_MAX / (597)));
		randomZ = -297 + (rand() / (RAND_MAX / (597)));
		while (coordinates.vertices[int(randomX)][int(randomZ)] == 600)
		{
			randomX = -297 + (rand() / (RAND_MAX / (597)));
			randomZ = -297 + (rand() / (RAND_MAX / (597)));
		}
		starsPos[i][0] = randomX;
		starsPos[i][2] = randomZ;
		starsPos[i][1] = coordinates.vertices[int(randomX)][int(randomZ)] + 12;
		isStar[i][0] = i;	//giving every star a unique index
		isStar[i][1] = 1;	//this is a flag, if it's 1, the star is rendered, if it's 0, it's not
		processMainMenu(RESTARTMENU);
	}
	timerMode = 0;
	max_time = 0;
}


void processTimeLevels(int option)
{
	switch (option)
	{
	case LVL1:
		//variables for number of stars in level
		numberOfStars = 16;
		max_time = 90;
		break;
	case LVL2:
		//variables for number of stars in level
		numberOfStars = 32;
		max_time = 60;
		break;
	case LVL3:
		//variables for number of stars in level
		numberOfStars = 50;
		max_time = 35;
		break;
	}
	//variables for stars collecting
	//numberOfStars = min_stars + (rand() * (int)(max_stars - min_stars) / RAND_MAX);
	starsPos.resize(numberOfStars, vector<int>(3));
	isStar.resize(numberOfStars, vector<int>(2));
	for (int i = 0; i < numberOfStars; ++i)
	{
		randomX = -297 + (rand() / (RAND_MAX / (597)));
		randomZ = -297 + (rand() / (RAND_MAX / (597)));
		while (coordinates.vertices[int(randomX)][int(randomZ)] == 600)
		{
			randomX = -297 + (rand() / (RAND_MAX / (597)));
			randomZ = -297 + (rand() / (RAND_MAX / (597)));
		}
		starsPos[i][0] = randomX;
		starsPos[i][2] = randomZ;
		starsPos[i][1] = coordinates.vertices[int(randomX)][int(randomZ)] + 12;
		isStar[i][0] = i;	//giving every star a unique index
		isStar[i][1] = 1;	//this is a flag, if it's 1, the star is rendered, if it's 0, it's not
		processMainMenu(RESTARTMENU);
	}
	timerMode = 1;
	timer = 0;
	old_timer = 0;
}


void emptymenu(int option)
{
	;
}


void createPopupMenus()
{
	//mode menu options
	easyTime = glutCreateMenu(processSpeedLevels);
	glutAddMenuEntry("Level 1", LVL1);
	glutAddMenuEntry("Level 2", LVL2);
	glutAddMenuEntry("Level 3", LVL3);

	hardTime = glutCreateMenu(processSpeedLevels);
	glutAddMenuEntry("Level 1", LVL1);
	glutAddMenuEntry("Level 2", LVL2);
	glutAddMenuEntry("Level 3", LVL3);

	easySpeed = glutCreateMenu(processTimeLevels);
	glutAddMenuEntry("Level 1", LVL1);
	glutAddMenuEntry("Level 2", LVL2);
	glutAddMenuEntry("Level 3", LVL3);

	hardSpeed = glutCreateMenu(processTimeLevels);
	glutAddMenuEntry("Level 1", LVL1);
	glutAddMenuEntry("Level 2", LVL2);
	glutAddMenuEntry("Level 3", LVL3);

	modesTime = glutCreateMenu(emptymenu);
	glutAddSubMenu("easy - normal mode", easyTime);
	glutAddSubMenu("hard - aggressive", hardTime);

	modesSpeed = glutCreateMenu(emptymenu);
	glutAddSubMenu("easy - normal mode", easySpeed);
	glutAddSubMenu("hard - aggressive", hardSpeed);

	modesCamera = glutCreateMenu(processmodesCamera);
	glutAddMenuEntry("Third person camera view", THIRDPERSON);
	glutAddMenuEntry("Distance camera view", DISTANCE);

	gameModes = glutCreateMenu(emptymenu);
	glutAddSubMenu("Beat the time records", modesSpeed);
	glutAddSubMenu("Collecting in limited time", modesTime);

	//main menu options
	mainMenu = glutCreateMenu(processMainMenu);
	glutAddMenuEntry("Restart the game round", RESTARTMENU);
	glutAddSubMenu("Change game mode", gameModes);
	glutAddSubMenu("Change camera display", modesCamera);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// this will allow us to know if the menu is active
	glutMenuStatusFunc(processMenuStatus);
}


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


void print(float x, float y, float z, const char* text)
{
	const char* c;
	glColor3d(0.0, 1.0, 0.0);
	glRasterPos3f(x, y*timerY, z);
	for (c = text; *(c + 4) != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}


void processKeyboardKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		if (pause == 0 && abs(timer - max_time) > 0.01)
		{
			if (speed < 1.5)	speed += 0.04;	// increasing speed till limit when moving forward, or decreasing speed when moving backwards
			if (turning_angle == 0)	//when turning angle is 0
			{
				rotation = 0;	//rover is not rotating
				angular_speed = 0;	//and agular speed is none
			}
		}
		break;
	case 's':
		if (pause == 0 && abs(timer - max_time) > 0.01)
		{
			if (speed > -1.0)	speed -= 0.04;	//decreasing speed till limit when moveing forward, or increasing speed when moving backwards
			if (turning_angle == 0)	//when turning angle is 0
			{
				rotation = 0;	//rover is not rotating
				angular_speed = 0;	//and agular speed is none
			}
		}
		break;
	case 'd':
		if (pause == 0 && abs(timer - max_time) > 0.01)
		{
			turning_angle += 4;	//increasing turning angle by 8 degrees
			rotation = 0.8 / tan(turning_angle / (180 / GL_PI));	//calculating the rotation of rover
			angular_speed = -speed / rotation;	//calculating the angular_speed
		}
		break;
	case 'a':
		if (pause == 0 && abs(timer - max_time) > 0.01)
		{
			turning_angle -= 4;	//decreasing turning angle by 8 degrees
			rotation = 0.8 / tan(turning_angle / (180 / GL_PI));	//calculating the rotation of rover
			angular_speed = -speed / rotation;	//calculating the angular_speed
		}
		break;
	case 'p':
		if (abs(timer - max_time) > 0.01)
		{
		if (pause == 1)	pause = 0;
		else pause = 1;
		}
		break;
	case 27:
		glutDestroyMenu(mainMenu);
		glutDestroyMenu(modesTime);
		exit(0);
		break;
	}
	if (key == 27)
		exit(0);
}


void timerCallback(int value)
{
	glutTimerFunc(100, timerCallback, 0);	//called every 100ms
	speed = 0.95 * speed;	//decreasing the speed by 5%
	diode_time += 1;	//iteration the diode glowing variable
	if (diode_time > 14)	diode_time = 0;		//every 1500ms reseting it to 0
}

void timerCallback2(int value)
{
	glutTimerFunc(10, timerCallback2, 0);	//called every 100ms

	if (abs(timer - max_time) > 0.01 && pause == 0)
	{
		if (timerMode == 1)		timer += 0.01;
		else timer -= 0.01;
		timerstring = to_string(timer);
		timerChars = timerstring.c_str();
	}
}

bool collisionDetection()
{
	//calculating the X position of rover
	backMiddle[0] += cos(rover_angle) * speed;
	backWheels[0] += cos(rover_angle) * speed;
	frontWheels[0] = pos_x  + cos(rover_angle) * 10;
	frontMiddle[0] = pos_x + cos(rover_angle) * 10;

	//calculating the Z position of rover
	backMiddle[2] += -sin(rover_angle) * speed;
	backWheels[2] += -sin(rover_angle) * speed;
	frontWheels[2] = pos_z  - sin(rover_angle) * 10;
	frontMiddle[2] = pos_z - sin(rover_angle) * 10;

	//calculating the camera position
	cameraPos[0] = pos_x + cos(rover_angle) * (-15.0) + cameraX;
	cameraPos[2] = pos_z - sin(rover_angle) * (-15.0) + cameraZ;
	cameraPos[1] = pos_y + 22.8;

	//checking if there is collision, and if is return 1
	if (coordinates.vertices[int(frontMiddle[0]) + 300][int(frontMiddle[2]) + 300] >= frontMiddle[1] - 1.2)	return 0;
	if (coordinates.vertices[int(backMiddle[0]) + 300][int(backMiddle[2]) + 300] >= backMiddle[1] - 1.2)	return 0;
	if (coordinates.vertices[int(frontWheels[0]) + 300][int(frontWheels[2]) + 300] >= frontWheels[1] + 1.5)	return 0;
	if (coordinates.vertices[int(backWheels[0]) + 300][int(backWheels[2]) + 300] >= backWheels[1] + 1.5)	return 0;

	//if there's no collision return 1
	return 1;
}


void StarDrawing(float starX, float starY, float starZ, int index)
{
	if (isStar[index][1] == 1)	//if star wasn't collected
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();	//stacking the object
		glColor3f(1.0, 0.827, 0);	//yellow colour

		glTranslatef(starX, starY, starZ);	//moving it back
		glRotatef(-antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the star
		glTranslatef(-starX, -starY, -starZ);	//moving the rotation center to star
		star.DrawStar(starX, starY, starZ);	//rendering the star
		glPopMatrix();	//unstacinkg the object


		//filling stars collecting table
		for (int i = starX - 3; i < starX + 3; i++)
		{
			for (int j = starZ - 3; j < starZ + 3; j++)
			{
				stars[i + 300][j + 300] = 597;	//changing Y coordinate for stars collecting table
			}
		}
	}
}

void starsCollecting()
{
	if (stars[int(frontMiddle[0]) + 300][int(frontMiddle[2]) + 300] >= frontMiddle[1])	//if front of rover meets star
	for (int i = 0; i < numberOfStars; i++)
	{
		if (abs(frontMiddle[0] - starsPos[i][0]) < 3 && abs(frontMiddle[2] - starsPos[i][2]) < 3)	isStar[i][1] = 0;	//star dissapears
	}
	if (stars[int(backMiddle[0]) + 300][int(backMiddle[2]) + 300] >= backMiddle[1])	//if back of rover meets star
	for (int i = 0; i < numberOfStars; i++)
	{
		if (abs(backMiddle[0] - starsPos[i][0]) < 3 && abs(backMiddle[2] - starsPos[i][2]) < 3)	isStar[i][1] = 0;	//star dissapears
	}
}


void renderScene(void)
{
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	// Reset transformations
	glLoadIdentity();

	rover_angle += angular_speed;	//calculating the angle by angular speed

	// Set the camera
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], x + frontWheels[0], y + cameraPos[1] - 25, z + frontWheels[2], 0.0f, 1.0f, 0.0f);
	
	//rendering ground with textures
	glColor3f(1.0, 1.0, 1.0);
	init(stbi_load("textures.jpg", &width, &height, &nrChannels, 0));
	textures.DrawT();
	glDeleteTextures(1, &textureName);
	
	//rendering object with textures
	init(stbi_load("stone.jpg", &width, &height, &nrChannels, 0));
	icosphere.DrawT();
	glDeleteTextures(1, &textureName);

	//rendering object without textures
	glColor3f(0, 0, 0);
	cube.Draw();

	//changing antennas' rotation variables
	antenna_angle += 0.02;
	antenna_rotation = 1 / tan(antenna_angle / (180 / GL_PI));

	canMove = collisionDetection();	//detecting possible collisions
	starsCollecting();	//checking if there is star collected

	//rendering all stars
	for (int i = 0; i < numberOfStars; i++)
	{
		StarDrawing(starsPos[i][0], starsPos[i][1] + 12, starsPos[i][2], isStar[i][0]);
	}

	
	//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA DAÆ TO DO SPRAWKA Z FABU£¥ AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	if (coordinates.vertices[int(frontWheels[0]) + 300][int(frontWheels[2]) + 300] < frontWheels[1] && coordinates.vertices[int(backWheels[0]) + 300][int(backWheels[2]) + 300] < backWheels[1])
	{
		//falling down if there is nothing below
		backMiddle[1] -= 0.01;
		backWheels[1] -= 0.01;
		frontWheels[1] -= 0.01;
		frontMiddle[1] -= 0.01;
		pos_y -= 0.01;
	}


	if (pause == 1)
	{
		glPushMatrix();
		print(frontWheels[0], frontWheels[1] + 18, frontWheels[2], timerChars);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(pos_x, 0, pos_z);	//moving the rover
		Lazik Marsjanski = Lazik();	//rendering the rover
		Marsjanski.antenna1();	//rendering the first antenne
		Marsjanski.antenna2();	//rendering the second antenne
		Marsjanski.diode(0.6);	//and showing it darker, if not
		glPopMatrix();	//unstacinkg the object
	}
	else
	{
		if (abs(timer - max_time) < 0.01)
		{
			glPushMatrix();
			print(frontWheels[0], frontWheels[1] + 18, frontWheels[2], "Tu beda wyniki    ");
			glPopMatrix();
		}
		else
		{

			glPushMatrix();
			print(frontWheels[0], frontWheels[1] + 18, frontWheels[2], timerChars);
			glPopMatrix();


			if (canMove == 1)	//if there's no collision detected
			{
				pos_x += cos(rover_angle) * speed;	//calculating the new X coordinate
				pos_z += -sin(rover_angle) * speed;	//calculating the new Z coordinate

				glPushMatrix();	//stacinkg the object
				glTranslatef(pos_x, 0, pos_z);	//moving the rotation center to rover
				glRotatef(rover_angle * (180 / GL_PI), 0, 1, 0);	//rotating the rover
				glTranslatef(-pos_x, 0, -pos_z);	//moving it back
				glTranslatef(pos_x, pos_y, pos_z);	//moving the rover
				Lazik Marsjanski = Lazik();	//rendering the rover
				//reseting the rotating variables, so rover moves only forward and backwards, when A and D are not clicked
				turning_angle = 0;
				rotation = 0;
				angular_speed = 0;

				glPushMatrix();	//stacinkg the object
				glTranslatef(9.0, 0, -2.0);	//moving the rotation center to wheel
				glRotatef(antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the antenne
				glTranslatef(-9.0, 0, 2.0);	//moving it back
				Marsjanski.antenna1();	//rendering the first antenne
				glPopMatrix();	//unstacinkg the object

				glPushMatrix();	//stacinkg the object
				glTranslatef(9.0, 0, 2.0);	//moving the rotation center to wheel
				glRotatef(-antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the antenne
				glTranslatef(-9.0, 0, -2.0);	//moving it back
				Marsjanski.antenna2();	//rendering the second antenne
				glPopMatrix();	//unstacinkg the object

				if (diode_time == 0)	Marsjanski.diode(0.9);	//diode red glowing when the variable is equal 0
				else	Marsjanski.diode(0.6);	//and showing it darker, if not

				glPopMatrix();	//unstacinkg the object
			}
			else	//if there is collision
			{
				pos_x -= 2 * cos(rover_angle) * speed;	//calculating the new X coordinate
				pos_z -= 2 * -sin(rover_angle) * speed;	//calculating the new Z coordinate

				speed = 0;

				glPushMatrix();	//stacinkg the object
				glTranslatef(pos_x, 0, pos_z);	//moving the rotation center to rover
				glRotatef(rover_angle * (180 / GL_PI), 0, 1, 0);	//rotating the rover
				glTranslatef(-pos_x, 0, -pos_z);	//moving it back
				glTranslatef(pos_x, 0, pos_z);	//moving the rover
				Lazik Marsjanski = Lazik();	//rendering the rover
				//reseting the rotating variables, so rover moves only forward and backwards, when A and D are not clicked
				turning_angle = 0;
				rotation = 0;
				angular_speed = 0;

				glPushMatrix();	//stacinkg the object
				glTranslatef(9.0, 0, -2.0);	//moving the rotation center to wheel
				glRotatef(antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the antenne
				glTranslatef(-9.0, 0, 2.0);	//moving it back
				Marsjanski.antenna1();	//rendering the first antenne
				glPopMatrix();	//unstacinkg the object

				glPushMatrix();	//stacinkg the object
				glTranslatef(9.0, 0, 2.0);	//moving the rotation center to wheel
				glRotatef(-antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the antenne
				glTranslatef(-9.0, 0, -2.0);	//moving it back
				Marsjanski.antenna2();	//rendering the second antenne
				glPopMatrix();	//unstacinkg the object

				if (diode_time == 0)	Marsjanski.diode(0.9);	//diode red glowing when the variable is equal 0
				else	Marsjanski.diode(0.6);	//and showing it darker, if not

				glPopMatrix();	//unstacinkg the object
			}
		}
	}
	
	//showing ground vertices
	/*
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
	*/

	//showing stars vertices
	/*
	for (int i = 0; i <= 600; i++)
	{
		for (int j = 0; j <= 600; j++)
		{
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_POINTS);
			glVertex3f(i - 300, stars[i][j], j - 300);
			glEnd();
		}
		cout << endl;
	}
	*/

	glutSwapBuffers();
}

int main(int argc, char** argv) 
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("£azik Marsjañski");
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	// here are the new entries
	glutKeyboardFunc(processKeyboardKeys);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	//Glut timer
	glutTimerFunc(100, timerCallback, 0);
	glutTimerFunc(100, timerCallback2, 0);
	// init Menus
	createPopupMenus();
		//correcting the collisions table, so objects also have collisions
	for (int i = 69; i <= 71; i++)
	{
		for (int j = -10; j <= 10; j++)	coordinates.vertices[i + 300][j + 300] = 600;
	}
	for (int i = -10; i <= 10; i++)
	{
		for (int j = -60; j <= -40; j++)	coordinates.vertices[i + 300][j + 300] = 600;
	}
	
	for (int i = 0; i < numberOfStars; ++i)
	{
		randomX = -297 + (rand() / (RAND_MAX / (597)));
		randomZ = -297 + (rand() / (RAND_MAX / (597)));
		while (coordinates.vertices[int(randomX)][int(randomZ)] == 600)
		{
			randomX = -297 + (rand() / (RAND_MAX / (597)));
			randomZ = -297 + (rand() / (RAND_MAX / (597)));
		}
		starsPos[i][0] = randomX;
		starsPos[i][2] = randomZ;
		starsPos[i][1] = coordinates.vertices[int(randomX)][int(randomZ)] + 12;
		isStar[i][0] = i;	//giving every star a unique index
		isStar[i][1] = 1;	//this is a flag, if it's 1, the star is rendered, if it's 0, it's not
	}

	// enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}