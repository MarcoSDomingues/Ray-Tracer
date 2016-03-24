#include "Scene.h"

Scene::Scene(std::string& filename) {
	parseFile(filename);
}

void Scene::parseBackground(std::stringstream& sin) {
	sin >> background[0] >> background[1] >> background[2];
	background[3] = 1.0f;
}

void Scene::parseFrom(std::stringstream& sin) {
	sin >> camera.eye.x >> camera.eye.y >> camera.eye.z;
}

void Scene::parseAt(std::stringstream& sin) {
	sin >> camera.at.x >> camera.at.y >> camera.at.z;
}

void Scene::parseUp(std::stringstream& sin) {
	sin >> camera.up.x >> camera.up.y >> camera.up.z;
}

void Scene::parseAngle(std::stringstream& sin) {
	sin >> camera.fovy;
}

void Scene::parseHither(std::stringstream& sin) {
	sin >> camera.hither;
}

void Scene::parseResolution(std::stringstream& sin) {
	sin >> camera.resolution.WinX >> camera.resolution.WinY;
}

void Scene::parseLight(std::stringstream& sin) {
	Light l;
	sin >> l.x >> l.y >> l.z >> l.r >> l.g >> l.b;
	lights.push_back(l);
}

void Scene::parseMaterial(std::stringstream& sin) {
	sin >> mat.r >> mat.g >> mat.b >> mat.kd >> mat.ks >> mat.shine >> mat.t >> mat.iof;
}

void Scene::parsePlane(std::stringstream& sin) {
	Point p1, p2, p3;
	sin >>
		p1.x >> p1.y >> p1.z >>
		p2.x >>p2.y >> p2.z >>
		p3.x >> p3.y >> p3.z;
	Plane *pl = new Plane(p1, p2, p3, mat);
	objects.push_back(pl);
}

void Scene::parseSphere(std::stringstream& sin) {
	Center center;
	float radius;
	
	sin >> center.x >> center.y >> center.z >> radius;

	Sphere *sphere = new Sphere(center, radius, mat);
	objects.push_back(sphere);
}

void Scene::parseLine(std::stringstream& sin) {
	std::string s;
	sin >> s;
	if (s.compare("b") == 0) parseBackground(sin);
	else if (s.compare("from") == 0) parseFrom(sin);
	else if (s.compare("at") == 0) parseAt(sin);
	else if (s.compare("up") == 0) parseUp(sin);
	else if (s.compare("angle") == 0) parseAngle(sin);
	else if (s.compare("hither") == 0) parseHither(sin);
	else if (s.compare("resolution") == 0) parseResolution(sin);

	else if (s.compare("l") == 0) parseLight(sin);
	else if (s.compare("f") == 0) parseMaterial(sin);
	else if (s.compare("pl") == 0) parsePlane(sin);
	else if (s.compare("s") == 0) parseSphere(sin);
}

const void Scene::parseFile(std::string& filename) {
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
}

Ray Scene::camGetPrimaryRay(int x, int y) {
    Vector3 ze = (camera.eye - camera.at).normalize();
    Vector3 xe = (camera.up.cross(ze)).normalize();
    Vector3 ye = ze.cross(xe);

    float df = (camera.eye - camera.at).length();

    float resX = camera.resolution.WinX;
    float resY = camera.resolution.WinY;

    float h = 2 * df * tan(camera.fovy / 2);
    float w = (resX / resY) * h;

    Ray ray;
    ray.origin = camera.eye;

    Vector3 dirX = w * (x / resX - 1 / 2) * xe;
    Vector3 dirY = h * (y / resY - 1 / 2) * ye;
    Vector3 dirZ = -df * ze;

    ray.direction = dirX + dirY + dirZ;

    return ray;
}
