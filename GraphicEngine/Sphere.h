#pragma once

#include "GeometricObject.h"
#include "Utils.h"

class Sphere : public GeometricObject {

private:

	Center center;
	float radius;

public:

	Sphere(const Center &c, float r, const Material &mat)
		: GeometricObject(mat), center(c), radius(r) {}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal);

};