#pragma once
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include "triangle_mesh.h"
#include "vectors.h"
#include "matrix.h"
#include "arg_parser.h"
#include <vector>

class Spline
{
public:
	Spline()
	{
	}
	Spline(int vertices_num)
	{
		this->vertices_num = vertices_num;
		vertices_array = new Vec3f[10000];
	}
	virtual ~Spline()
	{
		delete[] vertices_array;
	}
	// 用于画图的FOR VISUALIZATION
	virtual void Paint(ArgParser *args)
	{
		
	}
	// 用于实现样条类型转换的FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE *file) = 0;
	virtual void OutputBSpline(FILE *file) = 0;
	// 用于得到控制点的FOR CONTROL POINT PICKING
	virtual int getNumVertices()
	{
		return vertices_num;
	}
	virtual Vec3f getVertex(int i)
	{
		return vertices_array[i];
	}
	virtual void set(int index, Vec3f vector)
	{
		vertices_array[index] = vector;
	}
	// 用于编辑操作的FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y)
	{
		Vec3f vec = Vec3f(x, y, 0);
		vertices_array[selectedPoint] = vec;
	}
	virtual void addControlPoint(int selectedPoint, float x, float y)
	{
		Vec3f vec = Vec3f(x, y, 0);
		vertices_array[selectedPoint] = vec;
		vertices_num++;
	}
	virtual void deleteControlPoint(int selectedPoint)
	{
		for (int i = selectedPoint; i < vertices_num - 1; i++)
		{
			vertices_array[selectedPoint] = vertices_array[selectedPoint + 1];
		}
		vertices_num--;
	}
	// 用于产生三角形的FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser *args)
	{
		return 0;
	}
protected:
	int vertices_num;
	Vec3f* vertices_array;
};