#pragma once

#include "Utils.h"
#include "BBox.h"

class GeometricObject {

public:

	Material material;
	BBox bbox;

	GeometricObject(Material mat)
		: material(mat) {}

	virtual bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal) = 0;
	virtual BBox get_bounding_box() = 0;
};