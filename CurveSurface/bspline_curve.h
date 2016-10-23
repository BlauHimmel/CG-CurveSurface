#pragma once
#include "curve.h"

class BSplineCurve : public Curve
{
public:
	BSplineCurve()
	{

	}
	BSplineCurve(int num)
	{
		vertices_num = num;
		vertices_array = new Vec3f[10000];
		matrix_bezier = new float[16]{ 1.0f, -3.0f, 3.0f, -1.0f,
									   0.0f,  3.0f, -6.0f,  3.0f,
									   0.0f,  0.0f,  3.0f,  -3.0f,
									   0.0f,  0.0f,  0.0f,  1.0f };
		matrix_bspline = new float[16]{ 1.0f, -3.0f,  3.0f, -1.0f,
										4.0f,  0.0f, -6.0f,  3.0f,
										1.0f,  3.0f,  3.0f, -3.0f,
										0.0f,  0.0f,  0.0f,  1.0f };
	}
	virtual ~BSplineCurve()
	{
		delete[] matrix_bezier;
		delete[] matrix_bspline;
	}
	virtual void Paint(ArgParser* args)
	{
		Curve::Paint(args);
		for (int i = 0; i < vertices_num - 3; i ++)
		{
			bspline_4_control_point(i, args->curve_tessellation);
		}
	}
	virtual void OutputBezier(FILE *file)
	{
		Matrix bspline = (1.0f / 6.0f) * Matrix(matrix_bspline);
		Matrix bezier = Matrix(matrix_bezier);
		bezier.Inverse();
		Matrix transform = bspline * bezier;
		int num_splines = 0;
		Vec3f* splines[2500];
		for (int i = 0; i < vertices_num - 3; i++)
		{
			float g_matrix_array[12] = { vertices_array[i].x(), vertices_array[i + 1].x(), vertices_array[i + 2].x(), vertices_array[i + 3].x(),
										 vertices_array[i].y(), vertices_array[i + 1].y(), vertices_array[i + 2].y(), vertices_array[i + 3].y(),
								         vertices_array[i].z(), vertices_array[i + 1].z(), vertices_array[i + 2].z(), vertices_array[i + 3].z()};
			Matrix g_matrix = Matrix(g_matrix_array);
			Matrix result_matrix = g_matrix * transform;
			Vec3f vec_result[4] = { Vec3f(result_matrix.Get(0, 0), result_matrix.Get(0, 1), result_matrix.Get(0, 2)),
								    Vec3f(result_matrix.Get(1, 0), result_matrix.Get(1, 1), result_matrix.Get(1, 2)),
								    Vec3f(result_matrix.Get(2, 0), result_matrix.Get(2, 1), result_matrix.Get(2, 2)),
								    Vec3f(result_matrix.Get(3, 0), result_matrix.Get(3, 1), result_matrix.Get(3, 2)) };
			splines[num_splines] = vec_result;
			num_splines++;
		}
		for (int i = 0; i < num_splines; i++)
		{
			fprintf(file, "%s", "bezier num_vertices 4 ");
			fprintf(file, "%.1f %.1f %.1f ", splines[i][0].x(), splines[i][0].y(), splines[i][0].z());
			fprintf(file, "%.1f %.1f %.1f ", splines[i][1].x(), splines[i][1].y(), splines[i][1].z());
			fprintf(file, "%.1f %.1f %.1f ", splines[i][2].x(), splines[i][2].y(), splines[i][2].z());
			fprintf(file, "%.1f %.1f %.1f ", splines[i][3].x(), splines[i][3].y(), splines[i][3].z());
		}
	}
	virtual void OutputBSpline(FILE* file)
	{
		fprintf(file, "%s", "bspline num_vertices ");
		fprintf(file, "%d ", vertices_num);
		for (int i = 0; i < vertices_num; i++)
		{		
			fprintf(file, "%.1f %.1f %.1f ", vertices_array[i].x(), vertices_array[i].y(), vertices_array[i].z());
		}
	}
	virtual TriangleMesh* OutputTriangles(ArgParser *args)
	{
		int line_each_di = (float)vertices_num - 3.0f;
		int ver_num = (line_each_di * args->curve_tessellation + 1) * args->revolution_tessellation;
		int ver_tri = line_each_di * args->curve_tessellation * args->revolution_tessellation * 2;
		TriangleMesh* mesh = new TriangleMesh(ver_num, ver_tri);
		int k = 0;
		for (int p = 0; p < args->revolution_tessellation; p++)
		{
			Vec3f tmp_vec = Vec3f(9999.0f, 9999.0f, 9999.0f);
			float delta = ((float)p / (float)args->revolution_tessellation) * 360;
			Matrix tmp;
			Matrix rotation_matrix = tmp.MakeAxisRotation(Vec3f(0.0f, 1.0f, 0.0f), delta / 180 * 3.14159726);
			for (int i = 0; i < vertices_num - 3; i ++)
			{
				for (int j = 0; j <= args->curve_tessellation; j++)
				{
					GLfloat t = (GLfloat)j / ((GLfloat)args->curve_tessellation);
					Vec3f vec = bspline_equation(t, i);
					if (vec == tmp_vec)
					{
						continue;
					}
					tmp_vec.Set(vec.x(), vec.y(), vec.z());
					Matrix vec_matrix = Matrix();
					vec_matrix.Set(0, 0, vec.x());
					vec_matrix.Set(0, 1, vec.y());
					vec_matrix.Set(0, 2, vec.z());
					Matrix result_matrix = rotation_matrix * vec_matrix;
					Vec3f result = Vec3f(result_matrix.Get(0, 0), result_matrix.Get(0, 1), result_matrix.Get(0, 2));
					mesh->SetVertex(k, result);
					k++;
				}
			}
		}

		int l = 0;
		int vertex_each_circle = line_each_di * args->curve_tessellation + 1;
		for (int i = 0; i < ver_num; i++)
		{
			if ((i + 1) % vertex_each_circle != 0)
			{
				mesh->SetTriangle(l, i, (i + vertex_each_circle) % ver_num, (i + vertex_each_circle + 1) % ver_num);
				l++;
				mesh->SetTriangle(l, i, i + 1, (i + vertex_each_circle + 1) % ver_num);
				l++;
			}
		}
		return mesh;
	}
private:
	void bspline_4_control_point(int start_index, int step)
	{
		glColor3f(0.2, 0.9, 0.2);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= step; j++)
		{
			GLfloat t = (GLfloat)j / ((GLfloat)step);
			Vec3f vec = bspline_equation(t, start_index);
			glVertex3f(vec.x(), vec.y(), vec.z());		
		}
		glEnd();
	}
	Vec3f bspline_equation(GLfloat t, int start_index)
	{
		float t2 = t * t;
		float t3 = t * t * t;
		float b0 = (-1.0f * t3 + 3.0f * t2 - 3.0f * t + 1.0f) / 6.0f;
		float b1 = (3.0f * t3 - 6.0f * t2 + 4.0f) / 6.0f;
		float b2 = (-3.0f * t3 + 3.0f * t2 + 3.0f * t + 1.0f) / 6.0f;
		float b3 = t3 / 6.0f;
		float x = vertices_array[start_index].x() * b0 + vertices_array[start_index + 1].x() * b1 +
			vertices_array[start_index + 2].x() * b2 + vertices_array[start_index + 3].x() * b3;
		float y = vertices_array[start_index].y() * b0 + vertices_array[start_index + 1].y() * b1 +
			vertices_array[start_index + 2].y() * b2 + vertices_array[start_index + 3].y() * b3;
		float z = vertices_array[start_index].z() * b0 + vertices_array[start_index + 1].z() * b1 +
			vertices_array[start_index + 2].z() * b2 + vertices_array[start_index + 3].z() * b3;
		return Vec3f(x, y, z);
	}
	float* matrix_bspline;
	float* matrix_bezier;
};