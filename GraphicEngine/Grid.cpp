#include "Grid.h"

Vector3 Grid::min_coordinates() {
	BBox obj_bbox;

	Vector3 p0 = Vector3(kHugeValue);

	int num_objects = scene->objects.size();

	for (int i = 0; i < num_objects; i++) {
		obj_bbox = scene->objects[i]->get_bounding_box();

		if (obj_bbox.x0 < p0.x)
			p0.x = obj_bbox.x0;
		if (obj_bbox.y0 < p0.y)
			p0.y = obj_bbox.y0;
		if (obj_bbox.z0 < p0.z)
			p0.z = obj_bbox.z0;
	}

	p0.x -= kEpsilon; 
	p0.y -= kEpsilon;
	p0.z -= kEpsilon;

	return p0;
}

Vector3 Grid::max_coordinates() {
	BBox obj_bbox;

	Vector3 p1 = Vector3(-kHugeValue);

	int num_objects = scene->objects.size();

	for (int i = 0; i < num_objects; i++) {
		obj_bbox = scene->objects[i]->get_bounding_box();

		if (obj_bbox.x1 > p1.x)
			p1.x = obj_bbox.x1;
		if (obj_bbox.y1 > p1.y)
			p1.y = obj_bbox.y1;
		if (obj_bbox.z1 > p1.z)
			p1.z = obj_bbox.z1;
	}

	p1.x += kEpsilon;
	p1.y += kEpsilon;
	p1.z += kEpsilon;

	return p1;
}

float Grid::clamp(float c, float min, float max) {
	return (c < min ? min : (c > max ? max : c));
}

BBox Grid::get_bounding_box() {
	return bbox;
}

void Grid::setup_cells() {
	Vector3 p0 = min_coordinates();
	Vector3 p1 = max_coordinates();
	bbox.x0 = p0.x;
	bbox.x1 = p1.x;
	bbox.y0 = p0.y;
	bbox.y1 = p1.y;
	bbox.z0 = p0.z;
	bbox.z1 = p1.z;

	// compute the number of grid cells in the x, y, and z directions
	int num_objects = scene->objects.size();

	// dimensions of the grid in the x, y, and z directions

	double wx = p1.x - p0.x;
	double wy = p1.y - p0.y;
	double wz = p1.z - p0.z;

	double multiplier = 2.0;  	// multiplyer scales the number of grid cells relative to the number of objects

	double s = pow(wx * wy * wz / num_objects, 0.3333333);
	nx = multiplier * wx / s + 1;
	ny = multiplier * wy / s + 1;
	nz = multiplier * wz / s + 1;

	// set up the array of grid cells with null pointers

	
	int num_cells = nx * ny * nz;
	cells.reserve(num_objects);

	for (int j = 0; j < num_cells; j++)
		cells.push_back(NULL);

	// set up a temporary array to hold the number of objects stored in each cell

	std::vector<int> counts;
	counts.reserve(num_cells);

	for (int j = 0; j < num_cells; j++)
		counts.push_back(0);

	// put the objects into the cells

	BBox obj_bBox; 	// object's bounding box
	int index;  	// cell's array index

	for (int j = 0; j < num_objects; j++) {
		obj_bBox = scene->objects[j]->get_bounding_box();

		// compute the cell indices at the corners of the bounding box of the object

		int ixmin = clamp((obj_bBox.x0 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
		int iymin = clamp((obj_bBox.y0 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
		int izmin = clamp((obj_bBox.z0 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
		int ixmax = clamp((obj_bBox.x1 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
		int iymax = clamp((obj_bBox.y1 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
		int izmax = clamp((obj_bBox.z1 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);

		// add the object to the cells

		for (int iz = izmin; iz <= izmax; iz++) 					// cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) {			// cells in x direction
					index = ix + nx * iy + nx * ny * iz;

					if (counts[index] == 0) {
						cells[index] = scene->objects[j];
						counts[index] += 1;  						// now = 1
					}
					else {
						if (counts[index] == 1) {
							Compound* compound_ptr = new Compound;  // construct a compound object
							compound_ptr->add_object(cells[index]); // add object already in cell
							compound_ptr->add_object(scene->objects[j]);  	// add the new object
							cells[index] = compound_ptr;			// store compound in current cell
							counts[index] += 1;  					// now = 2
						}
						else {										// counts[index] > 1
							cells[index]->add_object(scene->objects[j]);	// just add current object
							counts[index] += 1;						// for statistics only
						}
					}
				}
	}  // end of for (int j = 0; j < num_objects; j++)


	   // erase the Compound::vector that stores the object pointers, but don't delete the objects

	objects.erase(objects.begin(), objects.end());


	// display some statistics on counts
	// this is useful for finding out how many cells have no objects, one object, etc
	// comment this out if you don't want to use it

	int num_zeroes = 0;
	int num_ones = 0;
	int num_twos = 0;
	int num_threes = 0;
	int num_greater = 0;

	for (int j = 0; j < num_cells; j++) {
		if (counts[j] == 0)
			num_zeroes += 1;
		if (counts[j] == 1)
			num_ones += 1;
		if (counts[j] == 2)
			num_twos += 1;
		if (counts[j] == 3)
			num_threes += 1;
		if (counts[j] > 3)
			num_greater += 1;
	}

	std::cout << "num_cells =" << num_cells << std::endl;
	std::cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << std::endl;
	std::cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << std::endl;

	// erase the temporary counts vector

	counts.erase(counts.begin(), counts.end());
}

bool Grid::checkIntersection(const Ray &ray, Vector3 &hitpoint, float &tmin, float &distance, Vector3 &normal, Material &m) {
	Material mm;
	double ox = ray.origin.x;
	double oy = ray.origin.y;
	double oz = ray.origin.z;
	double dx = ray.direction.x;
	double dy = ray.direction.y;
	double dz = ray.direction.z;

	float t;

	double x0 = bbox.x0;
	double y0 = bbox.y0;
	double z0 = bbox.z0;
	double x1 = bbox.x1;
	double y1 = bbox.y1;
	double z1 = bbox.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	// the following code includes modifications from Shirley and Morley (2003)

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return(false);


	// initial cell coordinates

	int ix, iy, iz;

	if (bbox.inside(ray.origin)) {  			// does the ray start inside the grid?
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		Vector3 p = ray.origin + t0 * ray.direction;  // initial hit point with grid's bounding box
		ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions

	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = kHugeValue;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = kHugeValue;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = kHugeValue;
		iz_step = -1;
		iz_stop = -1;
	}

	// traverse the grid

	Vector3 n;
	Vector3 hit;
	float dist;

	while (true) {
		GeometricObject* object_ptr = cells[ix + nx * iy + nx * ny * iz];
		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->checkIntersection(ray, hit, t, dist, n, mm) && t < tx_next) {
				hitpoint = hit;
				normal = n;
				tmin = t;
				distance = dist;
				m = mm;
				return true;
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->checkIntersection(ray, hit, t, dist, n, mm) && t < ty_next) {
					hitpoint = hit;
					normal = n;
					tmin = t;
					distance = dist;
					m = mm;
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
				if (object_ptr && object_ptr->checkIntersection(ray, hit, t, dist, n, mm) && t < tz_next) {
					hitpoint = hit;
					normal = n;
					tmin = t;
					distance = dist;
					m = mm;
					return (true);
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return (false);
			}
		}
	}
}