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
#define MAINMENU 2
#define TEXTURING 3

#define THIRDPERSON 1
#define DISTANCE 2

#define LVL1 1
#define LVL2 2
#define LVL3 3
#define LVL4 4
#define LVL5 5
#define LVL6 6
#define LVL7 7
#define LVL8 8
#define LVL9 9
#define LVL10 10
#define LVL11 11
#define LVL12 12

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
bool texturesFlag = 1;

//object for collecting points
Interpreter star = Interpreter("gwiazda.obj");
float stars[600][600];	//stars collecting table

//attacking object
Interpreter ufo = Interpreter("ufo.obj");
bool ufos[600][600];	//ufo attacking table

//variables for number of stars in level
int min_stars = 3;
int max_stars = 9;

//variables for stars collecting
int numberOfStars = min_stars + (rand() * (int)(max_stars - min_stars) / RAND_MAX);
vector<vector<int> > starsPos(numberOfStars, vector<int>(3));	//positions X, Y, Z
vector<vector<int> > isStar(numberOfStars, vector<int>(2));	//index and variable responsible for dissapearing
int starsCollected = 1;

int numberOfUfos = 0;
//variables for ufos attacks
vector<vector<int> > ufoPos(numberOfUfos, vector<int>(3));	//positions X, Y, Z and index
vector<vector<int> > ufoAttacks(numberOfUfos, vector<int>(2));	//1/-1 variables for X, Z iterations

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
float backWheels[] = { 0.0, 2.2, 0.0 };
float frontWheels[] = { 9.0, 2.2, 0.0 };

//camera additional variables
float cameraPos[] = { -15.0, 25.0, 0.0 };
float cameraX = 0.0;
float cameraZ = 0.0;

//collision checking variable
bool canMove = 1;

//variable giving different timer height for each camera mode
float timerY = 1.5;

//timer variables 
double timer = 0.0;

float old_timer = 0.0;
string timerstring = to_string(timer);
const char* timerChars = timerstring.c_str();

bool timerFlag = 0;
bool timerMode = 1;
int min_time = 0;

//pause and end-round flag
bool pause = 0;
bool endRound = 1;

//random 0/1 variable for ufos directions, ufo time for moving slower
int random = 0;
int ufoTime = 0;

//player HP, flag which says if it should be rendered, and text so it can be shown
float hp = 3;
const char* hpText = "HP: 3";
string ufostring;


//text for stars collecting
const char* starsC = "0";
string starsS;

//level
int level = 0;

//results table and it's writing out variables
float results[13][4];
bool sortOnce = 0;
const char* resultsC = "0";
string resultsS;




void sortResults()
{
	if (level > 6)
	{
		if (results[level][0] < results[level][1])
			swap(results[level][0], results[level][1]);
		if (results[level][0] < results[level][2])
			swap(results[level][0], results[level][2]);
		if (results[level][0] < results[level][3])
			swap(results[level][0], results[level][3]);
		if (results[level][1] < results[level][2])
			swap(results[level][1], results[level][2]);
		if (results[level][1] < results[level][3])
			swap(results[level][1], results[level][3]);
		if (results[level][2] < results[level][3])
			swap(results[level][2], results[level][3]);
	}
	else if (level > 0 && level < 7)
{
		if (results[level][0] > results[level][1])
			swap(results[level][0], results[level][1]);
		if (results[level][0] > results[level][2])
			swap(results[level][0], results[level][2]);
		if (results[level][0] > results[level][3])
			swap(results[level][0], results[level][3]);
		if (results[level][1] > results[level][2])
			swap(results[level][1], results[level][2]);
		if (results[level][1] > results[level][3])
			swap(results[level][1], results[level][3]);
		if (results[level][2] > results[level][3])
			swap(results[level][2], results[level][3]);
	}
	sortOnce = 1;
}



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
		hp = 3;

		turning_angle = 0.0;
		rover_angle = 0.0;
		rotation = 0.0;
		speed = 0.0;
		angular_speed = 0.0;
		pos_x = 0;
		pos_z = 0;
		pos_y = 2.2;

		backWheels[0] = 0.0;
		backWheels[1] = 2.2;
		backWheels[2] = 0.0;
		frontWheels[0] = 9.0;
		frontWheels[1] = 2.2;
		frontWheels[2] = 0.0;

		canMove = 1;

		sortOnce = 0;
		endRound = 0;
		old_timer = timer;

		starsCollected = 0;

		for (int i = 0; i < numberOfStars; i++)
		{
			isStar[i][1] = 1;	//creating stars back
		}
		break;
	case MAINMENU:
		endRound = 1;
		break;
	case TEXTURING:
		if (texturesFlag == 1)	texturesFlag = 0;
		else texturesFlag = 1;
		break;
	}
}


void processmodesCamera(int option)
{
	if (endRound == 0)
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
		numberOfStars = 5;
		timer = 90;
		numberOfUfos = 0;
		level = 1;
		break;
	case LVL2:
		numberOfStars = 10;
		timer = 80;
		numberOfUfos = 0;
		level = 2;
		break;
	case LVL3:
		numberOfStars = 15;
		timer = 75;
		numberOfUfos = 0;
		level = 3;
		break;
	case LVL4:
		//variables for levels
		numberOfStars = 5;
		timer = 90;
		numberOfUfos = 1;
		hp = 3;
		level = 4;
		break;
	case LVL5:
		numberOfStars = 10;
		timer = 80;
		numberOfUfos = 3;
		hp = 3;
		level = 5;
		break;
	case LVL6:
		numberOfStars = 15;
		timer = 75;
		numberOfUfos = 6;
		hp = 3;
		level = 6;
		break;
	}
	sortOnce = 0;
	endRound = 0;
	old_timer = timer;
	ufoPos.resize(numberOfUfos, vector<int>(3));
	ufoAttacks.resize(numberOfUfos, vector<int>(2));
	for (int i = 0; i < numberOfUfos; ++i)
	{
		randomX = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		randomZ = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		ufoPos[i][0] = randomX;
		ufoPos[i][2] = randomZ;
		ufoPos[i][1] = coordinates.vertices[int(randomX)][int(randomZ)] + 12;
		ufoAttacks[i][0] = random;
		if (random == 0) random = -1;
		random = (rand() * 2 / RAND_MAX);
		if (random == 0) random = -1;
		ufoAttacks[i][1] = random;
	}
	//variables for stars collecting
	starsPos.resize(numberOfStars, vector<int>(3));
	isStar.resize(numberOfStars, vector<int>(2));
	for (int i = 0; i < numberOfStars; ++i)
	{
		randomX = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		randomZ = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		while (coordinates.vertices[int(randomX)][int(randomZ)] == 600)
		{
			randomX = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
			randomZ = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		}
		starsPos[i][0] = randomX;
		starsPos[i][2] = randomZ;
		starsPos[i][1] = coordinates.vertices[int(randomX)][int(randomZ)] + 12;
		isStar[i][0] = i;	//giving every star a unique index
		isStar[i][1] = 1;	//this is a flag, if it's 1, the star is rendered, if it's 0, it's not
	}
	timerMode = 0;
	min_time = 0;
	processMainMenu(RESTARTMENU);
}


void processTimeLevels(int option)
{
	switch (option)
	{
	case LVL7:
		//variables for number of stars in level
		numberOfStars = 5;
		numberOfUfos = 0;
		level = 7;
		break;
	case LVL8:
		//variables for number of stars in level
		numberOfStars = 10;
		numberOfUfos = 0;
		level = 8;
		break;
	case LVL9:
		//variables for number of stars in level
		numberOfStars = 15;
		numberOfUfos = 0;
		level = 9;
		break;
	case LVL10:
		//variables for number of stars in level
		numberOfStars = 5;
		numberOfUfos = 1;
		hp = 3;
		level = 10;
		break;
	case LVL11:
		//variables for number of stars in level
		numberOfStars = 10;
		numberOfUfos = 3;
		hp = 3;
		level = 11;
		break;
	case LVL12:
		//variables for number of stars in level
		numberOfStars = 15;
		numberOfUfos = 6;
		hp = 3;
		level = 12;
		break;
	}
	sortOnce = 0;
	endRound = 0;
	min_time = -1;
	ufoPos.resize(numberOfUfos, vector<int>(3));
	ufoAttacks.resize(numberOfUfos, vector<int>(2));
	for (int i = 0; i < numberOfUfos; ++i)
	{
		randomX = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		randomZ = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		ufoPos[i][0] = randomX;
		ufoPos[i][2] = randomZ;
		ufoPos[i][1] = coordinates.vertices[int(randomX)][int(randomZ)] + 12;
		random = (rand() * 2 / RAND_MAX);
		ufoAttacks[i][0] = random;
		if (random == 0) random = -1;
		random = (rand() * 2 / RAND_MAX);
		if (random == 0) random = -1;
		ufoAttacks[i][1] = random;
	}
	//variables for stars collecting
	starsPos.resize(numberOfStars, vector<int>(3));
	isStar.resize(numberOfStars, vector<int>(2));
	for (int i = 0; i < numberOfStars; ++i)
	{
		randomX = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		randomZ = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		while (coordinates.vertices[int(randomX)][int(randomZ)] == 600)
		{
			randomX = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
			randomZ = ((250 + 250) * ((float)rand() / RAND_MAX)) -250;
		}
		starsPos[i][0] = randomX;
		starsPos[i][2] = randomZ;
		starsPos[i][1] = coordinates.vertices[int(randomX)][int(randomZ)] + 12;
		isStar[i][0] = i;	//giving every star a unique index
		isStar[i][1] = 1;	//this is a flag, if it's 1, the star is rendered, if it's 0, it's not
	}
	timerMode = 1;
	timer = 0;
	old_timer = 0;
	processMainMenu(RESTARTMENU);
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
	glutAddMenuEntry("Level 1", LVL4);
	glutAddMenuEntry("Level 2", LVL5);
	glutAddMenuEntry("Level 3", LVL6);

	easySpeed = glutCreateMenu(processTimeLevels);
	glutAddMenuEntry("Level 1", LVL7);
	glutAddMenuEntry("Level 2", LVL8);
	glutAddMenuEntry("Level 3", LVL9);

	hardSpeed = glutCreateMenu(processTimeLevels);
	glutAddMenuEntry("Level 1", LVL10);
	glutAddMenuEntry("Level 2", LVL11);
	glutAddMenuEntry("Level 3", LVL12);

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
	glutAddMenuEntry("Go to main menu", MAINMENU);
	glutAddMenuEntry("Restart the game round", RESTARTMENU);
	glutAddSubMenu("Change game mode", gameModes);
	glutAddSubMenu("Change camera display", modesCamera);
	glutAddMenuEntry("Disable/enable texturing", TEXTURING);

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
	glColor3d(1.0, 0.0, 0.0);
	glRasterPos3f(x, y, z);
	for (c = text; *(c + 4) != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
	glRasterPos3f(x, y - 2, z);
	for (c = starsC; *(c) != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
	glRasterPos3f(x, y - 4, z);
	for (c = hpText; *(c + 7) != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}


void printT(float x, float y, float z, const char* text)
{
	const char* c;
	glColor3d(1.0, 1.0, 1.0);
	glRasterPos3f(x, y, z);
	for (c = text; *(c + 4) != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}


void processKeyboardKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		if (pause == 0 && endRound == 0)
		{
			if (speed < 1.0)	speed += 0.02;	// increasing speed till limit when moving forward, or decreasing speed when moving backwards
			if (turning_angle == 0)	//when turning angle is 0
			{
				rotation = 0;	//rover is not rotating
				angular_speed = 0;	//and agular speed is none
			}
		}
		break;
	case 's':
		if (pause == 0 && endRound == 0)
		{
			if (speed > -0.75)	speed -= 0.02;	//decreasing speed till limit when moveing forward, or increasing speed when moving backwards
			if (turning_angle == 0)	//when turning angle is 0
			{
				rotation = 0;	//rover is not rotating
				angular_speed = 0;	//and agular speed is none
			}
		}
		break;
	case 'd':
		if (pause == 0 && endRound == 0)
		{
			turning_angle += 4;	//increasing turning angle by 8 degrees
			rotation = 0.8 / tan(turning_angle / (180 / GL_PI));	//calculating the rotation of rover
			angular_speed = -speed / rotation;	//calculating the angular_speed
		}
		break;
	case 'a':
		if (pause == 0 && endRound == 0)
		{
			turning_angle -= 4;	//decreasing turning angle by 8 degrees
			rotation = 0.8 / tan(turning_angle / (180 / GL_PI));	//calculating the rotation of rover
			angular_speed = -speed / rotation;	//calculating the angular_speed
		}
		break;
	case 'p':
		if (endRound == 0)
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
	ufoTime += 1;
	if (ufoTime == 5 || ufoTime == 25)		ufoTime = 0;
	if (timerMode == 1)		starsS = "Collected: " + to_string(starsCollected) + "/" + to_string(numberOfStars);
	else 		starsS = "Collected: " + to_string(starsCollected);
	starsC = starsS.c_str();
}


void timerCallback2(int value)
{
	glutTimerFunc(10, timerCallback2, 0);	//called every 100ms

	if (endRound == 0 && pause == 0)
	{
		if (timerMode == 1)		timer += 0.01;
		else timer -= 0.01;
		timerstring = to_string(timer);
		timerChars = timerstring.c_str();
		if (abs(timer - min_time) < 0.01) endRound = 1;
	}
}


bool collisionDetection()
{
	//calculating the X position of rover
	backWheels[0] += cos(rover_angle) * speed;
	frontWheels[0] = pos_x + cos(rover_angle) * 10;

	//calculating the Z position of rover
	backWheels[2] += -sin(rover_angle) * speed;
	frontWheels[2] = pos_z - sin(rover_angle) * 10;

	if (frontWheels[0] < -285)
	{
		frontWheels[0] = -285;
	}
	else if (frontWheels[0] > 285)
	{
		frontWheels[0] = 285;
	}
	if (frontWheels[2] < -285)
	{
		frontWheels[2] = -285;
	}
	else if (frontWheels[2] > 285)
	{
		frontWheels[2] = 285;
	}

	//calculating the camera position
	cameraPos[0] = pos_x + cos(rover_angle) * (-15.0) + cameraX;
	cameraPos[2] = pos_z - sin(rover_angle) * (-15.0) + cameraZ;
	cameraPos[1] = pos_y + 22.8;


	if (backWheels[0] < -285)
	{
		backWheels[0] = -285;
		pos_x = -285;
	}
	else if(backWheels[0] > 285)
	{
		backWheels[0] = 285;
		pos_x = 285;
	}

	if (backWheels[2] < -285)
	{
		backWheels[2] = -285;
		pos_z = -285;
	}
	else if (backWheels[2] > 285)
	{
		backWheels[2] = 285;
		pos_z = 285;
	}

	//checking if there is collision
	if (coordinates.vertices[int(frontWheels[0]) + 300][int(frontWheels[2]) + 300] == 600 || coordinates.vertices[int(backWheels[0]) + 300][int(backWheels[2]) + 300] == 600)	return 0;
	else if (coordinates.vertices[int(backWheels[0]) + 300][int(backWheels[2]) + 300] == 0 || coordinates.vertices[int(frontWheels[0]) + 300][int(frontWheels[2]) + 300] == 0)	return 1;
	if (frontWheels[1] > backWheels[1])
	{
		frontWheels[1] = coordinates.vertices[int(frontWheels[0]) + 300][int(frontWheels[2]) + 300] + 0.5;
		backWheels[1] = coordinates.vertices[int(frontWheels[0]) + 300][int(frontWheels[2]) + 300] + 0.5;
		pos_y = frontWheels[1] + 0.5;
		cameraPos[1] = pos_y + 23.2;
		return 1;
	}
	else
	{
		frontWheels[1] = coordinates.vertices[int(backWheels[0]) + 300][int(backWheels[2]) + 300] + 0.5;
		backWheels[1] = coordinates.vertices[int(backWheels[0]) + 300][int(backWheels[2]) + 300] + 0.5;
		pos_y = backWheels[1] + 0.5;
		cameraPos[1] = pos_y + 23.2;
		return 1;
	}

	//if there's no collision return 1
	return 1;
}



void starsCollecting()
{
	if (timerMode == 0 && starsCollected == numberOfStars)
	{
		starsPos.resize(numberOfStars+1, vector<int>(3));
		isStar.resize(numberOfStars+1, vector<int>(2));
		numberOfStars += 1;
		randomX = ((250 + 250) * ((float)rand() / RAND_MAX)) - 250;
		randomZ = ((250 + 250) * ((float)rand() / RAND_MAX)) - 250;
		starsPos[numberOfStars - 1][0] = randomX;
		starsPos[numberOfStars - 1][2] = randomZ;
		starsPos[numberOfStars - 1][1] = coordinates.vertices[int(randomX)][int(randomZ)] + 12;
		isStar[numberOfStars -1][0] = numberOfStars - 1;
		isStar[numberOfStars -1][1] = 1;
	}
	else if (timerMode == 1 && starsCollected == numberOfStars)	endRound = 1;
	else
	{
		if (stars[int(frontWheels[0]) + 300][int(frontWheels[2]) + 300] >= frontWheels[1])	//if front of rover meets star
			for (int i = 0; i < numberOfStars; i++)
			{
				if (abs(frontWheels[0] - starsPos[i][0]) < 5 && abs(frontWheels[2] - starsPos[i][2]) < 5 && isStar[i][1] == 1)
				{
					isStar[i][1] = 0;	//star dissapears
					starsCollected += 1;
				}
			}
		if (stars[int(backWheels[0]) + 300][int(backWheels[2]) + 300] >= backWheels[1])	//if back of rover meets star
			for (int i = 0; i < numberOfStars; i++)
			{
				if (abs(backWheels[0] - starsPos[i][0]) < 5 && abs(backWheels[2] - starsPos[i][2]) < 5 && isStar[i][1] == 1)
				{
					isStar[i][1] = 0;	//star dissapears
					starsCollected += 1;
				}
			}
	}
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
		star.DrawObj(starX, starY, starZ);	//rendering the star
		glPopMatrix();	//unstacinkg the object


		//filling stars collecting table
		for (int i = starX - 5; i < starX + 5; i++)
		{
			for (int j = starZ - 5; j < starZ + 5; j++)
			{
				stars[i + 300][j + 300] = 333;	//changing Y coordinate for stars attacking
			}
		}
	}
}


void ufoDrawing()
{
	for (int index = 0; index < numberOfUfos; index++)
	{

		if (ufoPos[index][0] > 283)	ufoAttacks[index][0] = -1;
		else if (ufoPos[index][0] < -283)	ufoAttacks[index][0] = 1;

		if (ufoPos[index][2] > 283)	ufoAttacks[index][1] = -1;
		else if (ufoPos[index][2] < -283)	ufoAttacks[index][1] = 1;

		if (ufoTime == 0)
		{
			ufoPos[index][2] += ufoAttacks[index][1];
			ufoPos[index][0] += ufoAttacks[index][0];
			ufoPos[index][1] = coordinates.vertices[ufoPos[index][0]][ufoPos[index][2]] + 12;
		}

		glPushMatrix();	//stacking the object
		glColor3f(7.0, 0.1, 0.5);
		glTranslatef(ufoPos[index][0], 12, ufoPos[index][2]);	//moving it back
		glRotatef( 2 * antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the ufo
		glTranslatef(-ufoPos[index][0], 12, -ufoPos[index][2]);	//moving the rotation center to ufo
		ufo.DrawObj(ufoPos[index][0], 12, ufoPos[index][2]);	//rendering the ufo
		glPopMatrix();	//unstacinkg the object
	}
}


void ufosAttacks()
{
	for (int index = 0; index < numberOfUfos; index++)
	{
		if (abs(ufoPos[index][0] - backWheels[0]) < 15 && abs(ufoPos[index][2] - backWheels[2]) < 15 || abs(ufoPos[index][0] - frontWheels[0]) < 15 && abs(ufoPos[index][2] - frontWheels[2]) < 15 || 
			(abs(ufoPos[index][0] - backWheels[0]) < 15 && abs(ufoPos[index][2] - backWheels[2]) < 15 && abs(ufoPos[index][0] - frontWheels[0]) < 15 && abs(ufoPos[index][2] - frontWheels[2])))
		{
			hp -= 1;
			ufoTime = 6;
		}
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
	
	if (texturesFlag == 1)
	{
		//rendering ground with textures
		glColor3f(1.0, 1.0, 1.0);
		init(stbi_load("stone.jpg", &width, &height, &nrChannels, 0));
		textures.DrawT();
		glDeleteTextures(1, &textureName);
	}
	else
	{
		//rendering ground without textures
		glColor3f(1.0, 1.0, 1.0);
		textures.DrawG();
	}

	//rendering object without textures
	glColor3f(0.5, 0.5, 0.5);
	icosphere.Draw();

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
		if (endRound == 1)
		{
			if (sortOnce == 0)
			{
				if (level > 6 && starsCollected == numberOfStars && timer > 0)
				{
					results[level][3] = timer;
					sortResults();
				}
				else if (level > 0 && level < 7 && starsCollected != 0)
				{
					results[level][0] = starsCollected;
					sortResults();
				}

				cout << level << endl;
				for (int j = 0; j < 4; j++)
				{
					cout << results[level][j] << " ";
				}
				cout << endl;
			}

			pos_x = 300;
			pos_z = 0;
			pos_y = 12.2;
			frontWheels[0] = 310;
			//calculating the camera position
			cameraPos[0] = 300;
			cameraPos[2] = 0;
			cameraPos[1] = 24.2 ;


			glPushMatrix();
			print(x + frontWheels[0], y + cameraPos[1] - 21, z + frontWheels[2], "Your score:    ");
			printT(x + frontWheels[0], y + cameraPos[1] - 16, z + frontWheels[2], "Martian Rover    ");
			printT(x + frontWheels[0], y + cameraPos[1] - 17, z + frontWheels[2] - 2, "Press right mouse button to open menu    ");
			printT(x + frontWheels[0], y + cameraPos[1] - 18, z + frontWheels[2], "Game modes:    ");
			printT(x + frontWheels[0], y + cameraPos[1] - 19, z + frontWheels[2] - 10, "Beat the time records in stars collecting    ");
			printT(x + frontWheels[0], y + cameraPos[1] - 19, z + frontWheels[2] + 5, "Collect as much stars as you can in limited time     ");
			//tutaj zaj�� si� tekstem, main strona, doda� wy�wietlanie wynik�w
			//dodatkowo do mod�w trzeba doda� zapisywanie wynik�w
			glPopMatrix();
		}
		else
		{
			x = 0;
			y = 15.0;

			glPushMatrix();
			print(frontWheels[0], frontWheels[1] + 15, frontWheels[2], timerChars);
			glPopMatrix();

			ufoDrawing();


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
				glTranslatef(9.0, 0, 2.0);	//moving it back
				glRotatef(-antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the antenne
				glTranslatef(-9.0, 0, -2.0);	//moving the rover to center
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
				glTranslatef(9.0, 0, -2.0);	////moving it back
				glRotatef(antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the antenne
				glTranslatef(-9.0, 0, 2.0);	//moving the rotation center to wheel
				Marsjanski.antenna1();	//rendering the first antenne
				glPopMatrix();	//unstacinkg the object

				glPushMatrix();	//stacinkg the object
				glTranslatef(9.0, 0, 2.0);	//moving it back 
				glRotatef(-antenna_angle * (180 / GL_PI), 0, 1, 0);	//rotating the antenne
				glTranslatef(-9.0, 0, -2.0);	//moving the rotation center to wheel
				Marsjanski.antenna2();	//rendering the second antenne
				glPopMatrix();	//unstacinkg the object

				if (diode_time == 0)	Marsjanski.diode(0.9);	//diode red glowing when the variable is equal 0
				else	Marsjanski.diode(0.6);	//and showing it darker, if not

				glPopMatrix();	//unstacinkg the object
			}
		}
	}
	
	if (ufoTime == 0)
	{
		ufosAttacks();
		ufostring = "HP: " + to_string(hp);
		hpText = ufostring.c_str();
		if (hp == 0)	endRound = 1;
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
	glutCreateWindow("�azik Marsja�ski");
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
		randomX = -285 + (rand() / (RAND_MAX / (585)));
		randomZ = -285 + (rand() / (RAND_MAX / (585)));
		while (coordinates.vertices[int(randomX)][int(randomZ)] == 600)
		{
			randomX = -285 + (rand() / (RAND_MAX / (585)));
			randomZ = -285 + (rand() / (RAND_MAX / (585)));
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