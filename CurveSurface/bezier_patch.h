#pragma once
#include "surface.h"

class BezierPatch : public Surface
{
public:
	BezierPatch()
	{
		vertices_num = 16;
		vertices_array = new Vec3f[10000];
	}
	virtual ~BezierPatch()
	{

	}
	virtual void OutputBezier(FILE *file)
	{

	}
	virtual void OutputBSpline(FILE *file)
	{
		
	}
	virtual void Paint(ArgParser *args)
	{
		
	}
	virtual TriangleMesh* OutputTriangles(ArgParser *args)
	{
		GLfloat vec[4][4][3] = {
			{ { vertices_array[0].x(), vertices_array[0].y(), vertices_array[0].z() }, { vertices_array[1].x(), vertices_array[1].y(), vertices_array[1].z() }, { vertices_array[2].x(), vertices_array[2].y(), vertices_array[2].z() }, { vertices_array[3].x(), vertices_array[3].y(), vertices_array[3].z() } },
			{ { vertices_array[4].x(), vertices_array[4].y(), vertices_array[4].z() }, { vertices_array[5].x(), vertices_array[5].y(), vertices_array[5].z() }, { vertices_array[6].x(), vertices_array[6].y(), vertices_array[6].z() }, { vertices_array[7].x(), vertices_array[7].y(), vertices_array[7].z() } },
			{ { vertices_array[8].x(), vertices_array[8].y(), vertices_array[8].z() }, { vertices_array[9].x(), vertices_array[9].y(), vertices_array[9].z() }, { vertices_array[10].x(), vertices_array[10].y(), vertices_array[10].z() }, { vertices_array[11].x(), vertices_array[11].y(), vertices_array[11].z() } },
			{ { vertices_array[12].x(), vertices_array[12].y(), vertices_array[12].z() }, { vertices_array[13].x(), vertices_array[13].y(), vertices_array[13].z() }, { vertices_array[14].x(), vertices_array[14].y(), vertices_array[14].z() }, { vertices_array[15].x(), vertices_array[15].y(), vertices_array[15].z() } }
		};
		TriangleMesh* mesh = new TriangleNet(args->patch_tessellation, args->patch_tessellation);
		int k = 0;
		for (int i = 0; i <= args->patch_tessellation; i++)
		{
			for (int j = 0; j <= args->patch_tessellation; j++)
			{
				float u = (float)i / ((float)args->patch_tessellation);
				float v = (float)j / ((float)args->patch_tessellation);
				Vec3f vv = bspline_equation(u, v, vec);
				mesh->SetVertex(k, vv);
				k++;
			}
		}
		return mesh;
	}
private:
	Vec3f bspline_equation(GLfloat u, GLfloat v, GLfloat vec[][4][3])
	{
		float b_u[4];
		b_u[0] = (1.0f - u) * (1.0f - u) * (1.0f - u);
		b_u[1] = 3.0f * u * (1.0f - u) * (1.0f - u);
		b_u[2] = 3.0f * u * u * (1.0f - u);
		b_u[3] = u * u * u;

		float b_v[4];
		b_v[0] = (1.0f - v) * (1.0f - v) * (1.0f - v);
		b_v[1] = 3.0f * v * (1.0f - v) * (1.0f - v);
		b_v[2] = 3.0f * v * v * (1.0f - v);
		b_v[3] = v * v * v;

		float p_v[4][3];
		p_v[0][0] = b_v[0] * vec[0][0][0] + b_v[1] * vec[0][1][0] + b_v[2] * vec[0][2][0] + b_v[3] * vec[0][3][0];
		p_v[0][1] = b_v[0] * vec[0][0][1] + b_v[1] * vec[0][1][1] + b_v[2] * vec[0][2][1] + b_v[3] * vec[0][3][1];
		p_v[0][2] = b_v[0] * vec[0][0][2] + b_v[1] * vec[0][1][2] + b_v[2] * vec[0][2][2] + b_v[3] * vec[0][3][2];

		p_v[1][0] = b_v[0] * vec[1][0][0] + b_v[1] * vec[1][1][0] + b_v[2] * vec[1][2][0] + b_v[3] * vec[1][3][0];
		p_v[1][1] = b_v[0] * vec[1][0][1] + b_v[1] * vec[1][1][1] + b_v[2] * vec[1][2][1] + b_v[3] * vec[1][3][1];
		p_v[1][2] = b_v[0] * vec[1][0][2] + b_v[1] * vec[1][1][2] + b_v[2] * vec[1][2][2] + b_v[3] * vec[1][3][2];

		p_v[2][0] = b_v[0] * vec[2][0][0] + b_v[1] * vec[2][1][0] + b_v[2] * vec[2][2][0] + b_v[3] * vec[2][3][0];
		p_v[2][1] = b_v[0] * vec[2][0][1] + b_v[1] * vec[2][1][1] + b_v[2] * vec[2][2][1] + b_v[3] * vec[2][3][1];
		p_v[2][2] = b_v[0] * vec[2][0][2] + b_v[1] * vec[2][1][2] + b_v[2] * vec[2][2][2] + b_v[3] * vec[2][3][2];

		p_v[3][0] = b_v[0] * vec[3][0][0] + b_v[1] * vec[3][1][0] + b_v[2] * vec[3][2][0] + b_v[3] * vec[3][3][0];
		p_v[3][1] = b_v[0] * vec[3][0][1] + b_v[1] * vec[3][1][1] + b_v[2] * vec[3][2][1] + b_v[3] * vec[3][3][1];
		p_v[3][2] = b_v[0] * vec[3][0][2] + b_v[1] * vec[3][1][2] + b_v[2] * vec[3][2][2] + b_v[3] * vec[3][3][2];

		float p_uv[3];
		p_uv[0] = b_u[0] * p_v[0][0] + b_u[1] * p_v[1][0] + b_u[2] * p_v[2][0] + b_u[3] * p_v[3][0];
		p_uv[1] = b_u[0] * p_v[0][1] + b_u[1] * p_v[1][1] + b_u[2] * p_v[2][1] + b_u[3] * p_v[3][1];
		p_uv[2] = b_u[0] * p_v[0][2] + b_u[1] * p_v[1][2] + b_u[2] * p_v[2][2] + b_u[3] * p_v[3][2];

		return Vec3f(p_uv[0], p_uv[1], p_uv[2]);
	}
};