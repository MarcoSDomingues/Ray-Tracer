#include "Sphere.h"

bool Sphere::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal) {

	//Vector3 sCenter = Vector3(center.x, center.y, center.z);
	Vector3 rayDirection = ray.direction;

	//normalize ray direction
	rayDirection = rayDirection.normalize();
	
	//calculate square distance between ray origen and sphere center
	float squareDistanceX = ((center.x - ray.origin.x) * (center.x - ray.origin.x));
	float squareDistanceY = ((center.y - ray.origin.y) * (center.y - ray.origin.y));
	float squareDistanceZ = ((center.z - ray.origin.z) * (center.z - ray.origin.z));

	float squareDistance = squareDistanceX + squareDistanceY + squareDistanceZ;
	distance = sqrt(squareDistance);

	//compare square distance with square sphere radius
	float squareRadius = radius * radius;
	
	if (squareDistance == squareRadius) {
		//ray origen on the sphere surface
		return false;
	}

	float Bx = ray.direction.x * (center.x - ray.origin.x);
	float By = ray.direction.y * (center.y - ray.origin.y);
	float Bz = ray.direction.z * (center.z - ray.origin.z);

	float B = Bx + By + Bz;

	if (squareDistance > squareRadius) {
		if (B < 0)
			return false;
	}

	float R = (B * B) - squareDistance + squareRadius;

	if (R < 0)
		return false;

	float t = 0.0f;
	if (squareDistance > squareRadius) {
		t = B - sqrt(R);
	}
	else if (squareDistance < squareRadius) {
		t = B + sqrt(R);
	}
	
	//Calculate intersection point
	hitpoint.x = ray.origin.x + ray.direction.x * t;
	hitpoint.y = ray.origin.y + ray.direction.y * t;
	hitpoint.z = ray.origin.z + ray.direction.z * t;

	//Calculate normal at hitpoint on sphere surface 
	normal.x = (hitpoint.x - center.x);
	normal.y = (hitpoint.y - center.y);
	normal.z = (hitpoint.z - center.z);

	normal = normal.normalize();

	return true;

}