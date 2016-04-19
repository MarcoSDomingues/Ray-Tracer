#pragma once

#include "GeometricObject.h"
#include "Utils.h"

class Sphere : public GeometricObject {

private:

	Center center;
	float radius;

public:

	Sphere(const Center &c, float r, const Material &mat)
		: GeometricObject(mat), center(c), radius(r) {
		boundingBoxMin = Vector3(center.x - radius, center.y - radius, center.z - radius);
		boundingBoxMax = Vector3(center.x + radius, center.y + radius, center.z + radius);
	}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal);

};