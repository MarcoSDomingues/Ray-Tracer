#pragma once

#include "Utils.h"

class GeometricObject {

public:

	Material material;

	GeometricObject(Material mat)
		: material(mat) {}

	virtual bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance) = 0;
};