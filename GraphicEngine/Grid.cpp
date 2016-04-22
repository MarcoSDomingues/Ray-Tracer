#include "Grid.h"

Vector3 Grid::min_coordinates() {
	BBox bbox;

	Vector3 p0 = Vector3(kHugeValue);

	int num_objects = scene->objects.size();

	for (int i = 0; i < num_objects; i++) {
		bbox = scene->objects[i]->get_bounding_box();

		if (bbox.x0 < p0.x)
			p0.x = bbox.x0;
		if (bbox.y0 < p0.y)
			p0.y = bbox.y0;
		if (bbox.z0 < p0.z)
			p0.z = bbox.z0;
	}

	p0.x -= kEpsilon; 
	p0.y -= kEpsilon;
	p0.z -= kEpsilon;

	return p0;
}

Vector3 Grid::max_coordinates() {
	BBox bbox;

	Vector3 p0 = Vector3(kHugeValue);

	int num_objects = scene->objects.size();

	for (int i = 0; i < num_objects; i++) {
		bbox = scene->objects[i]->get_bounding_box();

		if (bbox.x1 < p0.x)
			p0.x = bbox.x1;
		if (bbox.y1 < p0.y)
			p0.y = bbox.y1;
		if (bbox.z1 < p0.z)
			p0.z = bbox.z1;
	}

	p0.x += kEpsilon;
	p0.y += kEpsilon;
	p0.z += kEpsilon;

	return p0;
}

