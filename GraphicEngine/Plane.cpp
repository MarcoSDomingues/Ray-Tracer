#include "Plane.h"

bool Plane::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance) {
	Vector3 a = Vector3(p1.x, p1.y, p1.z);
	Vector3 b = Vector3(p2.x, p2.y, p2.z);
	Vector3 c = Vector3(p3.x, p3.y, p3.z);
	normal = ((b - a).cross(b - c)).normalize();

	// assuming vectors are all normalized
	float denom = normal.dot(ray.direction);
	if (denom > 1e-6) {
		Vector3 pointToRay = a - ray.origin;
		float auxD = pointToRay.dot(normal) / denom;
		if (auxD >= 0) {
			distance = auxD;
			hitpoint = ray.origin + ray.direction * auxD;
			return true;
		}
	}
	return false;
}