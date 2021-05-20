#include "Lazik.h"
#define GL_PI 3.14

Lazik::Lazik()
{
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


	axle(0.3f, 1.0f, 4.7f, 0.0f, -1.0f, 1.5f, 0.2, 0.2, 0.2, 20);
	axle(0.3f, 5.0f, 4.7f, 0.0f, -1.0f, 1.5f, 0.2, 0.2, 0.2, 20);
	axle(0.3f, 9.0f, 4.7f, 0.0f, -1.0f, 1.5f, 0.2, 0.2, 0.2, 20);

	wheel(1.0f, 1.0f, 3.2f, -2.0f, 1.0f, 0.0, 0.0, 0.0, 5);
	wheel(1.0f, 5.0f, 3.2f, -2.0f, 1.0f, 0.0, 0.0, 0.0, 5);
	wheel(1.0f, 9.0f, 3.2f, -2.0f, 1.0f, 0.0, 0.0, 0.0, 5);


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
}


void Lazik::cuboid(float x, float y, float z, float length, float height, float width, float red, float green, float blue)
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

void Lazik::cylinder(float R, float x, float y, float z, float length, float red, float green, float blue, int base, int mode)
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

void Lazik::axle(float R, float x, float y, float z, float length, float height, float red, float green, float blue, int base)
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

void Lazik::wheel(float R, float x, float y, float z, float length, float red, float green, float blue, int base)
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

	glColor3f(red + 0.1, green + 0.1f, blue + 0.1f);
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

void Lazik::cone(float R, float x, float y, float z, float height, float red, float green, float blue, int base)
{
	float alpha;	//k�t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(x, z, y);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(x + R * cos(alpha), z, y + R * sin(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(x + R * cos(0.0), z, y + R * sin(0.0));	//domkni�cie podstawy
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

void Lazik::antenna(float R, float x, float y, float z, float height, float red, float green, float blue, int base)
{
	float alpha;	//k�t
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);	//pocz�tek sklejanego prymitywu - g�rnej podstawy
	glVertex3f(z, y, x);	//koordynaty �rodka g�rnej podstawy
	for (alpha = 0.0; alpha < 2 * GL_PI; alpha += GL_PI / base)	//p�tla dla kolenych tr�jk�t�w tworz�cych podstaw�
	{
		glVertex3f(z + height, y + R * sin(alpha), x + R * cos(alpha));		//kolejne kraw�dzie zewn�trzne podstawy
	}
	glVertex3f(z + height, y + R * sin(0.0), x + R * cos(0.0));	//domkni�cie podstawy
	glEnd();	//koniec sklejanego prymitywu - g�rnej podstawy
}