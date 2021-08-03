#pragma once

// The projection matrices and formulas are from http://www.songho.ca/opengl/gl_projectionmatrix.html

#include "../deps/glm/glm.hpp"

namespace compgraphutils
{
	// Creates a 3x3 matrix containing a triangle vertices positions, counter-clockwise order, defined as
	// V1 ( 1.0f , -1.0f, -1.0f)
	// V2 ( 0.0f ,  1.0f, -1.0f)
	// V3 (-1.0f , -1.0f, -1.0f)
	glm::mat3 CreateTrianglePositions()
	{
		glm::mat3 Triangle = {
			 1.0f, -1.0f, -1.0f,
			 0.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f
		};

		return Triangle;
	}

	// Creates a standard perspective projection matrix using the planes parameters.
	// The near and far parameters MUST be positive values.
	glm::mat4 CreatePerspectiveMatrix(float r, float l, float t, float b, float n, float f)
	{
		glm::mat4 P;
		// Column-major ordering.
		P[0] = glm::vec4((2 * n)/(r - l), 0.0f, 0.0f, 0.0f);
		P[1] = glm::vec4(0.0f, (2 * n)/(t - b), 0.0f, 0.0f);
		P[2] = glm::vec4((r + l)/(r - l), (t + b) / (t - b), -(f + n) / (f - n), -1.0f);
		P[3] = glm::vec4(0.0f, 0.0f, (-2 * f * n)/(f - n), 0.0f);

		return P;
	}

	// Creates a standard orthographic projection matrix using the planes parameters.
	// The near and far parameters MUST be positive values.
	glm::mat4 CreateOrthoProjMatrix(float r, float l, float t, float b, float n, float f)
	{
		glm::mat4 P;
		// Column-major ordering.
		P[0] = glm::vec4(2/(r - l), 0.0f, 0.0f, 0.0f);
		P[1] = glm::vec4(0.0f, 2/(t - b), 0.0f, 0.0f);
		P[2] = glm::vec4(0.0f, 0.0f, -2/(f - n), 0.0f);
		P[3] = glm::vec4(-(r + l)/(r - l), -(t + b)/(t - b), -(f + n)/(f - n), 1.0f);

		return P;
	}

	// Project a point in eye-space to the near plane.
	glm::vec3 ProjectPointToNearPlane(const glm::vec3& point_e, float n, bool isPersProj = true)
	{
		if (isPersProj)
		{
			glm::vec3 point_p;
			point_p.x = (n * point_e.x) / (-point_e.z);
			point_p.y = (n * point_e.y) / (-point_e.z);
			point_p.z = n;

			return point_p;
		}
		else
			return point_e;

	}

	// Map a point from eye-space to clip-space.
	glm::vec4 MapPointToClipSpace(const glm::vec3 point_e, float r, float l, float t, float b, float n, float f, bool isPersProj = true)
	{
		glm::vec4 point_c;
		if (isPersProj)
		{
			point_c.x = ((2 * n * point_e.x) / (r - l)) + (((r + l) / (r - l)) * point_e.z);
			point_c.y = ((2 * n * point_e.y) / (t - b)) + (((t + b) / (t - b)) * point_e.z);
			point_c.z = (((f + n) / (f - n)) * (-point_e.z)) - ((2 * f * n) / (f - n));
			point_c.w = -point_e.z;
		}
		else
		{
			point_c.x = ((2 * point_e.x) / (r - l)) - (((r + l) / (r - l)));
			point_c.y = ((2 * point_e.y) / (t - b)) + (((t + b) / (t - b)));
			point_c.z = (((-2 * point_e.z) / (f - n)) - ((f + n) / (f - n)));
			point_c.w = 1;
		}

		return point_c;
	}

	// Map a point from clip-space to NDC-space.
	glm::vec3 NDCTransf(const glm::vec4 point_c)
	{
		glm::vec3 point_NDC;
		point_NDC.x = point_c.x / point_c.w;
		point_NDC.y = point_c.y / point_c.w;
		point_NDC.z = point_c.z / point_c.w;

		return point_NDC;
	}
}
