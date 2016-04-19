#pragma once

#include "GeometricObject.h"
#include "Utils.h"

class Plane : public GeometricObject {

private:

	Point p1, p2, p3;

public:

	Plane(const Point &a, const Point &b, const Point &c, const Material &mat)
		: GeometricObject(mat), p1(a), p2(b), p3(c) {
		boundingBoxMin = Vector3(p1.x, p1.y, p1.z);
		boundingBoxMax = Vector3(p3.x, p3.y, p3.z);
	}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal);

};