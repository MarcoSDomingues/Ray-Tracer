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
		bbox.x0 = vertices[0].x + kOffset;
		bbox.x1 = vertices[2].x - kOffset;
		bbox.y0 = vertices[0].y + kOffset;
		bbox.y1 = vertices[2].y - kOffset;
		bbox.z0 = vertices[0].z + kOffset;
		bbox.z1 = vertices[2].z - kOffset;
	}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal);

	BBox get_bounding_box();
};


