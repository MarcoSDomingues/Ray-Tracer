#pragma once

#include "GeometricObject.h"
#include "BBox.h"
#include "Utils.h"

class Plane : public GeometricObject {

private:

	Point p1, p2, p3;

public:

	Plane(const Point &a, const Point &b, const Point &c, const Material &mat)
		: GeometricObject(mat), p1(a), p2(b), p3(c) {
		bbox.x0 = -50;
		bbox.x1 = 50;
		bbox.y0 = -50;
		bbox.y1 = 50;
		bbox.z0 = -50;
		bbox.z1 = p1.z;
	}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &tmin, float &distance, Vector3 &normal, Material &m);

	BBox get_bounding_box();
	void add_object(GeometricObject* object_ptr) {}
};