#pragma once

#include <vector>

#include "GeometricObject.h"
#include "BBox.h"
#include "Scene.h"

class Grid {

private:

	std::vector<GeometricObject*> cells;
	BBox bbox;
	int nx, ny, nz;

	Scene *scene;

	Vector3 min_coordinates();
	Vector3 max_coordinates();

public:

	Grid(Scene* s) {
		scene = s;
	}

	BBox getBoundingBox();
	void setup_cells();

};