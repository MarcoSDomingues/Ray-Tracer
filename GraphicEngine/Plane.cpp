#include "Plane.h"

bool Plane::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal) {
	Vector3 a = Vector3(p1.x, p1.y, p1.z);
	Vector3 b = Vector3(p2.x, p2.y, p2.z);
	Vector3 c = Vector3(p3.x, p3.y, p3.z);

	Vector3 aux1 = b - a;
	Vector3 aux2 = c - a;

	normal = aux1.cross(aux2).normalize();

	float aux = normal.dot(ray.direction);

	if (aux == 0.0f) {
		return false;
	}
	//Vector3 origin = Vector3(0.0f, 0.0f, 0.0f);
	distance = (a - ray.origin).length();

	float t = -((normal.dot(ray.origin) + distance) / aux);

	if (t < 0.0f)
		return false;

	//Calculate intersection point
	hitpoint.x = ray.origin.x + ray.direction.x * t;
	hitpoint.y = ray.origin.y + ray.direction.y * t;
	hitpoint.z = ray.origin.z + ray.direction.z * t;

	return true;

}