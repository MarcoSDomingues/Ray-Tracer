#pragma once

#include "GeometricObject.h"
#include "Utils.h"

class Plane : public GeometricObject {

private:

	Point p1, p2, p3;

public:

	Plane(const Point &a, const Point &b, const Point &c, const Material &mat)
		: GeometricObject(mat), p1(a), p2(b), p3(c) {}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal);

};