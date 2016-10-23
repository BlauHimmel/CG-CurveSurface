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
private:
	Curve* curve;
};