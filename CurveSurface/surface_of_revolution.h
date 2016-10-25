#pragma once
#include "surface.h"

class SurfaceOfRevolution : public Surface
{
public:
	SurfaceOfRevolution(Curve* cur)
	{
		curve = cur;
	}
	virtual ~SurfaceOfRevolution()
	{

	}
	virtual void OutputBezier(FILE *file)
	{
		curve->OutputBezier(file);
	}
	virtual void OutputBSpline(FILE *file)
	{
		curve->OutputBSpline(file);
	}
	virtual void Paint(ArgParser *args)
	{
		curve->Paint(args);
	}
	virtual TriangleMesh* OutputTriangles(ArgParser *args)
	{
		return curve->OutputTriangles(args);
	}
	virtual int getNumVertices()
	{
		return curve->getNumVertices();
	}
	virtual Vec3f getVertex(int i)
	{
		return curve->getVertex(i);
	}
	virtual void moveControlPoint(int selectedPoint, float x, float y)
	{
		curve->moveControlPoint(selectedPoint, x, y);
	}
	virtual void addControlPoint(int selectedPoint, float x, float y)
	{
		curve->addControlPoint(selectedPoint, x, y);
	}
	virtual void deleteControlPoint(int selectedPoint)
	{
		curve->deleteControlPoint(selectedPoint);
	}
private:
	Curve* curve;
};