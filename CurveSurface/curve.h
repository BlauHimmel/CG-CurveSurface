#pragma once
#include "spline.h"
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
class Curve : public Spline
{
public:
	Curve()
	{
	}
	Curve(int num)
	{
		vertices_num = vertices_num;
		vertices_array = new Vec3f[10000];
	}
	virtual ~Curve()
	{
	}
	virtual void Paint(ArgParser *args)
	{
		drawVertices();
		drawLines();
	}
	void drawVertices()
	{
		for (int i = 0; i < vertices_num; i++)
		{
			float v1 = 0, v2 = 0, v3 = 0;
			vertices_array[i].Get(v1, v2, v3);
			glPointSize(5.0);
			glBegin(GL_POINTS);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(v1, v2, v3);
			glEnd();
		}
	}
	void drawLines()
	{
		for (int i = 0; i < vertices_num - 1; i++)
		{
			float v1 = 0, v2 = 0, v3 = 0;
			vertices_array[i].Get(v1, v2, v3);
			float v4 = 0, v5 = 0, v6 = 0;
			vertices_array[i + 1].Get(v4, v5, v6);
			glBegin(GL_LINES);
			glLineWidth(10);
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(v1, v2, v3);
			glVertex3f(v4, v5, v6);
			glEnd();
		}
	}
};