#pragma once

#include "Utils.h"

class GeometricObject {

public:

	Material material;
	Vector3 normal = Vector3(0.0f, 0.0f, 0.0f);

	GeometricObject(Material mat)
		: material(mat) {}

	virtual bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance) {
		return false;
	}
};