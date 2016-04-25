#pragma once

#pragma once

#include <vector>
#include "GeometricObject.h"
#include "Utils.h"
#include "Vectors.h"

#define EPS 0.0001

class Poly : public GeometricObject {

public:
	
	std::vector<Vector3> vertices;

	Poly(const Material &mat)
		: GeometricObject(mat) {
	}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &tmin, float &distance, Vector3 &normal, Material &m);

	BBox get_bounding_box();
	void add_object(GeometricObject* object_ptr) {}

	void setBoundingBox() {
		bbox.x0 = min(vertices[0].x, vertices[1].x, vertices[2].x);
		bbox.x1 = max(vertices[0].x, vertices[1].x, vertices[2].x);
		bbox.y0 = min(vertices[0].y, vertices[1].y, vertices[2].y);
		bbox.y1 = max(vertices[0].y, vertices[1].y, vertices[2].y);
		bbox.z0 = min(vertices[0].z, vertices[1].z, vertices[2].z);
		bbox.z1 = max(vertices[0].z, vertices[1].z, vertices[2].z);
	}

	float max(float a, float b, float c) {
		if (a > b && a > c) {
			return a;
		}
		else if (b > c) {
			return b;
		}
		else {
			return c;
		}
	}

	float min(float a, float b, float c) {
		if (a < b && a < c) {
			return a;
		}
		else if (b < c) {
			return b;
		}
		else {
			return c;
		}
	}
};


