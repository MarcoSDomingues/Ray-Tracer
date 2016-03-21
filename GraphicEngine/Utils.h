#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Vectors.h"

typedef struct {
	float r, g, b;
} Color;

typedef struct {
	Vector3 origin;
	Vector3 direction;
} Ray;

typedef struct {
	int WinX, WinY;
} Resolution;

typedef struct {
	float x, y, z;
} Center;

typedef struct {
	float x, y, z, r, g, b;
} Light;

typedef struct {
	float r, g, b, kd, ks, shine, t, iof;
} Material;

typedef struct {
	float x, y, z;
} Point;

typedef struct {
	Vector3 eye, at, up;
	float fovy;
	float hither;
	Resolution resolution;
} Camera;


