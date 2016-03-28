#pragma once

#pragma once

#include <vector>
#include "GeometricObject.h"
#include "Utils.h"
#include "Vectors.h"

class Poly : public GeometricObject {

public:
	
	std::vector<Vector3> vertices;

	Poly(const Material &mat)
		: GeometricObject(mat) {}

	bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal);

};
