#include "Vertices.h"

Vertices::Vertices(string filename)
{
	ifstream file(filename);
	string line;
	while (getline(file, line))
	{
		if (line.substr(0, 1) == "v")
		{
			vector<GLfloat> Point;
			GLfloat x, y, z;
			istringstream s(line.substr(2));
			s >> x; s >> y; s >> z;
			Point.push_back(x);
			Point.push_back(y);
			Point.push_back(z);
			v.push_back(Point);
		}
		if (line.substr(0, 1) == "f")
		{
			vector<GLint> vIndexSets;
			GLint u, v, w;
			istringstream vtns(line.substr(2));
			vtns >> u; vtns >> v; vtns >> w;
			vIndexSets.push_back(u - 1);
			vIndexSets.push_back(v - 1);
			vIndexSets.push_back(w - 1);
			f.push_back(vIndexSets);
		}
	}
	file.close();
	for (int i = 0; i < f.size(); i++)
	{
		//Three vertices
		vertex a, b, c;
		GLint firstVertexIndex = (f[i])[0];//Remove the vertex index
		GLint secondVertexIndex = (f[i])[1];
		GLint thirdVertexIndex = (f[i])[2];

		a.x = (v[firstVertexIndex])[0];//The first vertex
		a.y = (v[firstVertexIndex])[1];
		a.z = (v[firstVertexIndex])[2];

		b.x = (v[secondVertexIndex])[0]; //The second vertex
		b.y = (v[secondVertexIndex])[1];
		b.z = (v[secondVertexIndex])[2];

		c.x = (v[thirdVertexIndex])[0]; //The third vertex
		c.y = (v[thirdVertexIndex])[1];
		c.z = (v[thirdVertexIndex])[2];

		//looking for points inside every triangle
		IsInside(a.x, b.x, c.x, a.z, b.z, c.z, a.y, b.y, c.y);
	}

}


void Vertices::IsInside(float x1, float x2, float x3, float z1, float z2, float z3, float y1, float y2, float y3)
{
	min(x1, x2, x3, z1, z2, z3);	//looking for the smallest X position
	max(x1, x2, x3, z1, z2, z3);	//looking for higher X position
	middle(x1, x2, x3, z1, z2, z3);	//looking for middle X position

	//calculating the Z coordinate of point which is on X posisition of middle vertex and it's Z position is on straight line which is passing through first and last vertex
	middleZ = ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * minmax[1][0] + minmax[0][1] - ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * minmax[0][0];

	if (middleZ < minmax[1][1])	//if middle vertex is above straight line which is passing through first and last vertex
	{
		for (int i = minmax[0][0]; i <= minmax[1][0]; i++)//i are X positions on tringle from first vertex to middle one
		{
			//for X position there are below calculated minimum, and maximum values using equation of a line passing through two points - first and middle vertex for max, first and last for min
			Zmin = ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * i + minmax[0][1] - ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * minmax[0][0];
			Zmax = ((minmax[0][1] - minmax[1][1]) / (minmax[0][0] - minmax[1][0])) * i + minmax[0][1] - ((minmax[0][1] - minmax[1][1]) / (minmax[0][0] - minmax[1][0])) * minmax[0][0];
			for (int j = Zmin; j <= Zmax; j++)	//j are Z positions in tringle
			{
				//calculating the Y position of given X and Z using barycentric coordinate system
				float det = (z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3);
				float l1 = ((z2 - z3) * (i - x3) + (x3 - x2) * (j - z3)) / det;
				float l2 = ((z3 - z1) * (i - x3) + (x1 - x3) * (j - z3)) / det;
				float l3 = 1.0f - l1 - l2;
				vertices[i + 300][j + 300] = l1 * y1 + l2 * y2 + l3 * y3;	//saving Y position of Vertex in table
			}
		}
		for (int i = minmax[1][0] + 1; i <= minmax[2][0]; i++)//i are now X positions on triangle from middle vertex to lest vertex
		{
			//for X position there are below calculated minimum, and maximum values using equation of a line passing through two points - first and middle vertex for max, first and last for min
			Zmin = ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * i + minmax[0][1] - ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * minmax[0][0];
			Zmax = ((minmax[1][1] - minmax[2][1]) / (minmax[1][0] - minmax[2][0])) * i + minmax[1][1] - ((minmax[1][1] - minmax[2][1]) / (minmax[1][0] - minmax[2][0])) * minmax[1][0];
			for (int j = Zmin; j <= Zmax; j++)
			{
				//calculating the Y position of given X and Z using barycentric coordinate system
				float det = (z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3);
				float l1 = ((z2 - z3) * (i - x3) + (x3 - x2) * (j - z3)) / det;
				float l2 = ((z3 - z1) * (i - x3) + (x1 - x3) * (j - z3)) / det;
				float l3 = 1.0f - l1 - l2;
				vertices[i + 300][j + 300] = l1 * y1 + l2 * y2 + l3 * y3;	//saving Y position of Vertex in table
			}
		}
	}
	else
	{
		for (int i = minmax[0][0]; i <= minmax[1][0]; i++)//i are X positions on tringle from first vertex to middle one
		{
			//for X position there are below calculated minimum, and maximum values using equation of a line passing through two points
			// this time it's: first and middle vertex for min, first and last for max
			Zmax = ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * i + minmax[0][1] - ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * minmax[0][0];
			Zmin = ((minmax[0][1] - minmax[1][1]) / (minmax[0][0] - minmax[1][0])) * i + minmax[0][1] - ((minmax[0][1] - minmax[1][1]) / (minmax[0][0] - minmax[1][0])) * minmax[0][0];
			for (int j = Zmin; j <= Zmax; j++)
			{
				//calculating the Y position of given X and Z using barycentric coordinate system
				float det = (z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3);
				float l1 = ((z2 - z3) * (i - x3) + (x3 - x2) * (j - z3)) / det;
				float l2 = ((z3 - z1) * (i - x3) + (x1 - x3) * (j - z3)) / det;
				float l3 = 1.0f - l1 - l2;
				vertices[i + 300][j + 300] = l1 * y1 + l2 * y2 + l3 * y3;	//saving Y position of Vertex in table
			}
		}
		for (int i = minmax[1][0] + 1; i <= minmax[2][0]; i++)//i are now X positions on triangle from middle vertex to lest vertex
		{
			//for X position there are below calculated minimum, and maximum values using equation of a line passing through two points
			// this time it's: middle and last vertex for min, first and last for max
			Zmax = ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * i + minmax[0][1] - ((minmax[0][1] - minmax[2][1]) / (minmax[0][0] - minmax[2][0])) * minmax[0][0];
			Zmin = ((minmax[1][1] - minmax[2][1]) / (minmax[1][0] - minmax[2][0])) * i + minmax[1][1] - ((minmax[1][1] - minmax[2][1]) / (minmax[1][0] - minmax[2][0])) * minmax[1][0];
			for (int j = Zmin; j <= Zmax; j++)
			{
				//calculating the Y position of given X and Z using barycentric coordinate system
				float det = (z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3);
				float l1 = ((z2 - z3) * (i - x3) + (x3 - x2) * (j - z3)) / det;
				float l2 = ((z3 - z1) * (i - x3) + (x1 - x3) * (j - z3)) / det;
				float l3 = 1.0f - l1 - l2;
				vertices[i + 300][j + 300] = l1 * y1 + l2 * y2 + l3 * y3;	//saving Y position of Vertex in table
			}
		}
	}

}


void Vertices::min(float x1, float x2, float x3, float z1, float z2, float z3)
{
	//vertex with minimal X value is saved on first position in minmax table
	if (x1 <= x2 && x1 <= x3)
	{
		minmax[0][0] = x1;
		minmax[0][1] = z1;
	}
	else if (x2 <= x1 && x2 <= x3)
	{
		minmax[0][0] = x2;
		minmax[0][1] = z2;
	}
	else
	{
		minmax[0][0] = x3;
		minmax[0][1] = z3;
	}
}

void Vertices::max(float x1, float x2, float x3, float z1, float z2, float z3)
{
	//vertex with max X value is saved on last position in minmax table
	if (x1 >= x2 && x1 >= x3)
	{
		minmax[2][0] = x1;
		minmax[2][1] = z1;
	}
	else if (x2 >= x1 && x2 >= x3)
	{
		minmax[2][0] = x2;
		minmax[2][1] = z2;
	}
	else
	{
		minmax[2][0] = x3;
		minmax[2][1] = z3;
	}
}

void Vertices::middle(float x1, float x2, float x3, float z1, float z2, float z3)
{
	//vertex with middle X value is saved on middle position in minmax table
	if (x1 >= x2 && x1 <= x3)
	{
		minmax[1][0] = x1;
		minmax[1][1] = z1;
	}
	else if (x1 <= x2 && x1 >= x3)
	{
		minmax[1][0] = x1;
		minmax[1][1] = z1;
	}
	else if (x2 >= x1 && x2 <= x3)
	{
		minmax[1][0] = x2;
		minmax[1][1] = z2;
	}
	else if (x2 <= x1 && x2 >= x3)
	{
		minmax[1][0] = x2;
		minmax[1][1] = z2;
	}
	else
	{
		minmax[1][0] = x3;
		minmax[1][1] = z3;
	}
}