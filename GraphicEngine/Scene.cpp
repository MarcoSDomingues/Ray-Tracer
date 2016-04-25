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
	if (verticesCount <= 0)
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
	else if (s.compare("p") == 0) {
		sin >> verticesCount;
		Poly *poly = new Poly(mat);
		polys.push_back(poly);
		polyID++;
	}

	else if (verticesCount > 0) {
		Vector3 vertex;
		sin >> vertex.x >> vertex.y >> vertex.z;
		polys[polyID]->vertices.push_back(vertex);

		if (verticesCount == 1) {
			objects.push_back(polys[polyID]);
			polys[polyID]->setBoundingBox();
		}

		verticesCount--;
	}
}

float Scene::getDF() {
	Vector3 eye = camera.eye;
	Vector3 at = camera.at;
	Vector3 ze = (eye - at).normalize();
	return ze.length();
}

const void Scene::parseFile(std::string& filename) {
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
}

Vector3 Scene::getZe() {
	Vector3 eye = camera.eye;
	Vector3 at = camera.at;
	Vector3 up = camera.up;

	return (eye - at).normalize();
}

Vector3 Scene::getXe() {
	Vector3 eye = camera.eye;
	Vector3 at = camera.at;
	Vector3 up = camera.up;

	Vector3 ze = (eye - at).normalize();
	return up.cross(ze).normalize();
}

Vector3 Scene::getYe() {
	Vector3 eye = camera.eye;
	Vector3 at = camera.at;
	Vector3 up = camera.up;

	//Camera Frame - XeYeZe (uvn)
	Vector3 ze = (eye - at).normalize();
	Vector3 xe = up.cross(ze).normalize();
	return ze.cross(xe);
}

Ray Scene::camGetPrimaryRay(int x, int y) {

	Vector3 eye = camera.eye;
	Vector3 at = camera.at;
	Vector3 up = camera.up;

	//Camera Frame - XeYeZe (uvn)
	Vector3 ze = (eye - at).normalize();
	Vector3 xe = up.cross(ze).normalize();
	Vector3 ye = ze.cross(xe);


	//calculate df, h and w

	float resX = camera.resolution.WinX;
	float resY = camera.resolution.WinY;

	float df = (eye - at).length();

	float h = 2 * df * tan(camera.fovy / 2);
	float w = (resX / resY) * h;

	Ray ray;
	ray.origin = eye;

	ray.direction = -df * ze + h * ((y / resY) - 0.5) * ye + w * ((x / resX) - 0.5) * xe;
	ray.direction = ray.direction.normalize();

	return ray;
}
