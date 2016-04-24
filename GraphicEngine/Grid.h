#pragma once

#include <vector>

#include "GeometricObject.h"
#include "BBox.h"
#include "Scene.h"
#include "Compound.h"

class Grid : public Compound {

private:

	std::vector<GeometricObject*> cells;
	BBox bbox;
	int nx, ny, nz;

	Scene *scene;

	Vector3 min_coordinates();
	Vector3 max_coordinates();

public:

	Grid(Scene* s) : Compound() {
		scene = s;
		nx = 0;
		ny = 0;
		nz = 0;
	}

	virtual BBox get_bounding_box();
	void setup_cells();

	float clamp(float x, float min, float max);

	virtual bool checkIntersection(const Ray &ray, Vector3 &hitpoint, float &tmin, float &distance, Vector3 &normal);
	void add_object(GeometricObject* object_ptr) {}

};