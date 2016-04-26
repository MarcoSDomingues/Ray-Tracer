#include "Compound.h"

Compound::Compound() {
	material.r = 0.0f; material.g = 0.0f; material.b = 0.0f; material.kd = 0.0f;
	material.ks = 0.0f; material.shine = 0.0f; material.t = 0.0f; material.iof = 0.0f;
}

void Compound::add_object(GeometricObject* object_ptr) {
	objects.push_back(object_ptr);
}

BBox Compound::get_bounding_box() {
	return bbox;
}

bool Compound::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &tmin, float &distance, Vector3 &normal, Material &m) {
	float t, dist;
	bool hit = false;
	tmin = kHugeValue;
	int num_objects = objects.size();

	Material mm;
	Vector3 hit_point, n;

	//std::cout << "lele" << std::endl;
	for (int j = 0; j < num_objects; j++) {
		if (objects[j]->checkIntersection(ray, hit_point, t, dist, n, mm) && (t < tmin)) {
			hit = true;
			tmin = t;
			normal = n;
			distance = dist;
			hitpoint = hit_point;
			m = mm;
		}
	}

	return hit;
}

int Compound::get_num_objects(void) {
	return objects.size();
}