#include "DepthOfField.h"
#include <random>

DepthOfField::DepthOfField(float fD, float focalDistanceRatio, float lR, int samples) {
	focalDistance = fD * focalDistanceRatio;
	lensRadius = lR;
	numOfSamples = samples;
	//zoom = z;
	//distance = d;
}
