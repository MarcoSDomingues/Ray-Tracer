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
		bbox.x0 = center.x - radius;
		bbox.x1 = center.x + radius;
		bbox.y0 = center.y - radius;
		bbox.y1 = center.y + radius;
		bbox.z0 = center.z - radius;
		bbox.z1 = center.z + radius;
	}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &tmin, float &distance, Vector3 &normal, Material &m);

	BBox get_bounding_box();
	void add_object(GeometricObject* object_ptr) {}
};