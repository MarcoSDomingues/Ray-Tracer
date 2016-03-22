#include "Sphere.h"

bool Sphere::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance) {
	//if there is any problem we should try to normalize vectors
	Vector3 sphereCenter = Vector3(center.x, center.y, center.z);
	Vector3 vpc = ray.origin - sphereCenter; // vector from ray.origin to sphere.center
	Vector3 pc = ray.origin + sphereCenter * ray.direction; //projection of the center of the sphere in the ray 

	if ((vpc.dot(ray.direction)) < 0) { // when the sphere is behind the origin p
		if (vpc.length() > radius) { // there is no intersection
			return false;
		}
		else if (vpc.length() == radius) { // one hitPoint
			hitpoint = ray.origin;
		}
		else { // occurs when ray.origin is inside the sphere
			float temp = (pc - sphereCenter).length();
			float dist = sqrt(radius * radius - temp * temp);
			distance = dist - (pc - ray.origin).length();
			hitpoint = ray.origin + ray.direction * distance;
		}
	}
	else { // center of sphere projects on the ray
		if ((sphereCenter - pc).length() > radius) { // there is no intersection
			return false;
		}
		else {
			float temp = (pc - sphereCenter).length();
			float dist = sqrt(radius * radius - temp * temp);
			if (vpc.length() > radius) { // origin is outside sphere	
				distance = (pc - ray.origin).length() - dist;
			}
			else { // origin is inside sphere
				distance = (pc - ray.origin).length() + dist;
			}
			hitpoint = ray.origin + ray.direction * distance;
		}
	}
	return true;
}