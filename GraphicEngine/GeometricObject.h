#pragma once

#include "Utils.h"
#include "BBox.h"

class GeometricObject {

public:

	Material material;
	BBox bbox;

	GeometricObject() {}
	GeometricObject(Material mat)
		: material(mat) {}

	virtual bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &tmin, float &distance, Vector3 &normal, Material &m) = 0;
	virtual BBox get_bounding_box() = 0;
	virtual void add_object(GeometricObject* object_ptr) = 0;
};