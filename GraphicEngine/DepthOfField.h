#pragma once

#include "Utils.h"
#include "Scene.h"

class DepthOfField {

public:

	float focalDistance; //focal plane distance
	float lensRadius; // lens radius
	float zoom; // zoom factor
	float distance; // view plane distance
	int numOfSamples;

	DepthOfField(float fD, float focalDistanceRatio, float lR, int samples);
};