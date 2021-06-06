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

		IsInside(a.x, b.x, c.x, a.z, b.z, c.z, a.y, b.y, c.y);
	}

}

void Vertices::IsInside(int x1, int x2, int x3, int z1, int z2, int z3, int y1, int y2, int y3)
{
	float minZ = min(z1, z2, z3);
	float maxZ = max(z1, z2, z3);
	float minX = min(x1, x2, x3);
	float maxX = max(x1, x2, x3);

	for (int i = minX; i <= maxX; i++)
	{
		for (int j = minZ; j <= maxZ; j++)
		{
			double T = Obszar(x1, z1, x2, z2, x3, z3);
			double T1 = Obszar(i, j, x2, z2, x3, z3);
			double T2 = Obszar(x1, z1, i, j, x3, z3);
			double T3 = Obszar(x1, z1, x2, z2, i, j);
			{
				float det = (z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3);
				float l1 = ((z2 - z3) * (i - x3) + (x3 - x2) * (j - z3)) / det;
				float l2 = ((z3 - z1) * (i - x3) + (x1 - x3) * (j - z3)) / det;
				float l3 = 1.0f - l1 - l2;
				vertices[i + 300][j + 300] = l1 * y1 + l2 * y2 + l3 * y3;
			}
		}
	}
}

bool Vertices::Obszar(int x1, int z1, int x2, int z2, int x3, int z3)
{
	return 0.5 * abs((x1 * (z2 - z3) + x2 * (z3 - z1) + x3 * (z1 - z2)));
}

float Vertices::min(float v1, float v2, float v3)
{
	if (v1 <= v2 && v1 <= v3)	return v1;
	else if (v2 <= v1 && v2 <= v3)	return v2;
	else	return v3;
}

float Vertices::max(float v1, float v2, float v3)
{
	if (v1 >= v2 && v1 >= v2)	return v1;
	else if (v2 >= v1 && v2 >= v3)	return v2;
	else	return v3;
}