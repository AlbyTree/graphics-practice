// The projection matrices and formulas are from http://www.songho.ca/opengl/gl_projectionmatrix.html

#include "Utils.h"
#include <vector>
#include "../deps/glm/glm.hpp"

glm::mat3 compgraphutils::CreateTrianglePositions()
{
	glm::mat3 Triangle = {
		 1.0f, -1.0f, -1.0f,
		 0.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
	};

	return Triangle;
}

glm::mat4 compgraphutils::CreatePerspectiveMatrix(float r, float l, float t, float b, float n, float f)
{
	glm::mat4 P;
	// Column-major ordering.
	P[0] = glm::vec4((2 * n)/(r - l), 0.0f, 0.0f, 0.0f);
	P[1] = glm::vec4(0.0f, (2 * n)/(t - b), 0.0f, 0.0f);
	P[2] = glm::vec4((r + l)/(r - l), (t + b) / (t - b), -(f + n) / (f - n), -1.0f);
	P[3] = glm::vec4(0.0f, 0.0f, (-2 * f * n)/(f - n), 0.0f);

	return P;
}

glm::mat4 compgraphutils::CreateOrthoProjMatrix(float r, float l, float t, float b, float n, float f)
{
	glm::mat4 P;
	// Column-major ordering.
	P[0] = glm::vec4(2/(r - l), 0.0f, 0.0f, 0.0f);
	P[1] = glm::vec4(0.0f, 2/(t - b), 0.0f, 0.0f);
	P[2] = glm::vec4(0.0f, 0.0f, -2/(f - n), 0.0f);
	P[3] = glm::vec4(-(r + l)/(r - l), -(t + b)/(t - b), -(f + n)/(f - n), 1.0f);

	return P;
}

glm::vec3 compgraphutils::ProjectPointToNearPlane(const glm::vec3& point_e, float n, bool isPersProj)
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

glm::vec4 compgraphutils::MapPointToClipSpace(const glm::vec3 point_e, float r, float l, float t, float b, float n, float f, bool isPersProj)
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

glm::vec3 compgraphutils::NDCTransf(const glm::vec4 point_c)
{
	glm::vec3 point_NDC;
	point_NDC.x = point_c.x / point_c.w;
	point_NDC.y = point_c.y / point_c.w;
	point_NDC.z = point_c.z / point_c.w;

	return point_NDC;
}

void compgraphutils::generateCurvePointsBez(int numCurvePoints, const glm::vec3* CPS, std::vector<glm::vec3>& curvePoints)
{
	int numSegments = numCurvePoints - 1;
	for (int i = 0; i <= numSegments; ++i)
	{
		float t = i / (float)numSegments;

		// Compute coefficients
		float k1 = (1 - t) * (1 - t) * (1 - t);
		float k2 = 3 * (1 - t) * (1 - t) * t;
		float k3 = 3 * (1 - t) * t * t;
		float k4 = t * t * t;

		// Weight the four control points using coefficients
		curvePoints[i] = ((CPS[0] * k1 + CPS[1] * k2 + CPS[2] * k3 + CPS[3] * k4));
	}
}

void compgraphutils::generateCurvePointsDecast(int numCurvePoints, const glm::vec3* CPS, std::vector<glm::vec3>& curvePoints)
{
	int numSegments = numCurvePoints - 1;
	for (int i = 0; i <= numSegments; ++i)
	{
		float t = i / (float)numSegments;

		// Compute first tree points along main segments P1-P2, P2-P3 and P3-P4
		glm::vec3 CP12 = (1 - t) * CPS[0] + t * CPS[1];
		glm::vec3 CP23 = (1 - t) * CPS[1] + t * CPS[2];
		glm::vec3 CP34 = (1 - t) * CPS[2] + t * CPS[3];

		// Compute two points along segments P1P2-P2P3 and P2P3-P3P4
		glm::vec3 CP1223 = (1 - t) * CP12 + t * CP23;
		glm::vec3 CP2334 = (1 - t) * CP23 + t * CP34;

		// Finally compute P
		curvePoints[i] = ((1 - t) * CP1223 + t * CP2334);
	}
}
