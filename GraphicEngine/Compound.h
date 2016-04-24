#pragma once

#include <vector> 

#include "GeometricObject.h"
#include "Utils.h"
#include <vector>

class Compound : public GeometricObject {

protected:
	std::vector<GeometricObject*> objects;

public:

	Compound();

	virtual void add_object(GeometricObject* object_ptr);

	int get_num_objects(void);
	BBox get_bounding_box();

	virtual bool checkIntersection(const Ray &ray, Vector3 &hitpoint,float &tmin, float &distance, Vector3 &normal);

};