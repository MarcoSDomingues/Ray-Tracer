#include "Poly.h"

bool Poly::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &distance, Vector3 &normal) {

	Vector3 a = (vertices[1] - vertices[0]);
	Vector3 b = (vertices[2] - vertices[0]);

	Vector3 n = a.cross(b).normalize();
	float d = -vertices[0].dot(n);

	int i0, i1, i2;

	float tDenom = n.dot(ray.direction);
	if (tDenom == 0.0)
		return false;

	float tAux = n.dot(ray.origin) + d;
	float t = -(tAux / tDenom);

	if (n.x > n.y) {
		if (n.x > n.z) {
			i0 = 0; i1 = 1; i2 = 2;
		}
		else {
			i0 = 2; i1 = 0; i2 = 1;
		}
	}
	else {
		if (n.y < n.z) {
			i0 = 2; i1 = 0; i2 = 1;
		}
		else {
			i0 = 1; i1 = 0; i2 = 2;
		}
	}

	//Calculate intersection point
	hitpoint.x = ray.origin.x + ray.direction.x * t;
	hitpoint.y = ray.origin.y + ray.direction.y * t;
	hitpoint.z = ray.origin.z + ray.direction.z * t;

	float P[3] = { hitpoint.x, hitpoint.y, hitpoint.z };
	float V0[3] = { vertices[0].x, vertices[0].y ,vertices[0].z };
	float V1[3] = { vertices[1].x, vertices[1].y ,vertices[1].z };
	float V2[3] = { vertices[2].x, vertices[2].y ,vertices[2].z };

	Vector3 u, v;

	u.x = P[i1] - V0[i1];
	u.y = V1[i1] - V0[i1];
	u.z = V2[i1] - V0[i1];

	v.x = P[i2] - V0[i2];
	v.y = V1[i2] - V0[i2];
	v.z = V2[i2] - V0[i2];

	float beta, alpha;
	if (u.y == 0.0f) {
		beta = u.x / u.z;
		if (beta < 0.0 || beta > 1.0)
			return false;
		alpha = (v.x - beta * v.z) / v.y;
	}
	else {
		beta = (v.x * u.y - u.x * v.y) / (v.z * u.y - u.z * v.y);
		if (beta < 0.0 || beta > 1.0)
			return false;
		alpha = (u.x - beta * u.z) / u.y;
	}

	normal = n;
	distance = (ray.origin - hitpoint).length();
	

	return (alpha >= 0.0f && beta >= 0.0f && (alpha + beta) <= 1.0f);
}

BBox Poly::get_bounding_box() {
	return bbox;
}