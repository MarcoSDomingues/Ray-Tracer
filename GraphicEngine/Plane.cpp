#include "Plane.h"

bool Plane::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance) {
	Vector3 a = Vector3(p1.x, p1.y, p1.z);
	Vector3 b = Vector3(p2.x, p2.y, p2.z);
	Vector3 c = Vector3(p3.x, p3.y, p3.z);
	Vector3 normal = ((b - a).cross(c - a)).normalize();
	Vector3 tempOrigin = ray.origin;
	Vector3 rayOrig = tempOrigin.normalize();
	Vector3 tempDirection = ray.direction;
	Vector3 rayDir = tempDirection.normalize();

	// assuming vectors are all normalized
	float denom = normal.dot(rayDir);
	if (denom > 1e-6) {
		Vector3 pointToRay = a - rayOrig;
		float auxD = pointToRay.dot(normal) / denom;
		if (auxD >= 0) {
			distance = auxD;
			hitpoint = rayOrig + rayDir * auxD;
			return true;
		}
	}
	return false;
}