#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"

#include "GeometricObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "Utils.h"

class Scene {

public:

	float background[4];

	Camera camera;

	Material mat;

	std::vector<Light> lights;
	std::vector<GeometricObject*> objects;

	Scene(std::string& filename);

	void parseBackground(std::stringstream& sin);
	void parseFrom(std::stringstream& sin);
	void parseAt(std::stringstream& sin);
	void parseUp(std::stringstream& sin);
	void parseAngle(std::stringstream& sin);
	void parseHither(std::stringstream& sin);
	void parseResolution(std::stringstream& sin);

	void parseLight(std::stringstream& sin);
	void parseMaterial(std::stringstream& sin);
	void parsePlane(std::stringstream& sin); 
	void parseSphere(std::stringstream& sin);

	void parseLine(std::stringstream& sin);
	const void parseFile(std::string& filename);

	//calculate Primary Rays
	Ray camGetPrimaryRay(int x, int y);

};