#pragma once

// The projection matrices and formulas were taken from http://www.songho.ca/opengl/gl_projectionmatrix.html

#include <vector>
#include "glm/glm.hpp"

namespace compgraphutils
{
    struct AABB
    {
        AABB() : min(-1, -1, -1), max(1, 1, 1) {}
        AABB(const glm::vec3& point, const float ray) : min(point.x - ray, point.y - ray, point.z - ray), max(point.x + ray, point.y + ray, point.z + ray) {}
        glm::vec3 min;
        glm::vec3 max;
    };

    struct Ray
    {
        Ray() : position(), direction(glm::vec3(0,0,1)) {}
        Ray(const glm::vec3& p, const glm::vec3& d) : position(p), direction(glm::normalize(d)) {}
        glm::vec3 position;
        glm::vec3 direction;
    };

	float RayCast(const Ray& ray, const AABB& aabb);
	bool RayCast(const Ray& ray, const AABB& aabb, float& t);
	bool RayCast(const Ray& ray, const AABB& aabb, glm::vec3& point);
	
	// Creates a 3x3 matrix containing a triangle vertices positions, counter-clockwise order, defined as
	// V1 ( 1.0f , -1.0f, -1.0f)
	// V2 ( 0.0f ,  1.0f, -1.0f)
	// V3 (-1.0f , -1.0f, -1.0f)
	//
	glm::mat3 CreateTrianglePositions();

	// Creates a standard perspective projection matrix using the planes parameters.
	// The near and far parameters MUST be positive values.
	glm::mat4 CreatePerspectiveMatrix(float r, float l, float t, float b, float n, float f);

	// Creates a standard orthographic projection matrix using the planes parameters.
	// The near and far parameters MUST be positive values.
	glm::mat4 CreateOrthoProjMatrix(float r, float l, float t, float b, float n, float f);

	// Project a point in eye-space to the near plane.
	glm::vec3 ProjectPointToNearPlane(const glm::vec3& point_e, float n, bool isPersProj = true);

	// Map a point from eye-space to clip-space.
	glm::vec4 MapPointToClipSpace(const glm::vec3 point_e, float r, float l, float t, float b, float n, float f, bool isPersProj = true);

	// Map a point from clip-space to NDC-space.
	glm::vec3 NDCTransf(const glm::vec4 point_c);

	// Generate points of a Bezier curve using 4 control points.
	// `CPS` must have exactly 4 vectors.
	// `curvePoints` must have exactly numCurvePoints size.
	void generateCurvePointsBez(int numCurvePoints, const glm::vec3* CPS, std::vector<glm::vec3>& curvePoints);

	// Generate points of a De Casteljau curve using 4 control points.
	// `CPS` must have exactly 4 vectors.
	// `curvePoints` must have exactly numCurvePoints size.
	void generateCurvePointsDecast(int numCurvePoints, const glm::vec3* CPS, std::vector<glm::vec3>& curvePoints);
}
