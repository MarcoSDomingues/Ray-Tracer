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
		bbox.x0 = p1.x + kOffset;
		bbox.x1 = p3.x - kOffset;
		bbox.y0 = p1.y + kOffset;
		bbox.y1 = p3.y - kOffset;
		bbox.z0 = p1.z + kOffset;
		bbox.z1 = p3.z - kOffset;
	}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal);

	BBox get_bounding_box();
};