#include "Sphere.h"

bool Sphere::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance) {

	Vector3 sphereCenter = Vector3(center.x, center.y, center.z).normalize();
	Vector3 vpc = sphereCenter - ray.origin; // vector from ray.origin to sphere.center
	Vector3 pc = ray.origin + ((ray.direction - ray.origin).dot(vpc) / (ray.direction - ray.origin).length()) * (ray.direction - ray.origin); //projection of the center of the sphere in the ray 

	if (vpc.dot(ray.direction) < 0) { // when the sphere is behind the origin p
		if (vpc.length() > radius) { // there is no intersection
			std::cout << "entrei";
			return false;
		}
		else if (vpc.length() == radius) { // one hitPoint
			hitpoint = ray.origin;
			normal = (hitpoint - sphereCenter).normalize();
		}
		else { // occurs when ray.origin is inside the sphere
			float temp = (pc - sphereCenter).length();
			float dist = sqrt(radius * radius - temp * temp);
			distance = dist - (pc - ray.origin).length();
			hitpoint = ray.origin + ray.direction * distance;
			normal = (hitpoint - sphereCenter).normalize();
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
			normal = (hitpoint - sphereCenter).normalize();
		}
	}
	return false;
}