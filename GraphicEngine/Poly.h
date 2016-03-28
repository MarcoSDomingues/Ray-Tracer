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
		: GeometricObject(mat) {}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal);

};
