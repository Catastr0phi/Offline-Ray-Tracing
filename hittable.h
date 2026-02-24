#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include <DirectXMath.h>

class HitRecord {
public:
	XMFLOAT3 p;
	XMFLOAT3 normal;
	double t;
};

class Hittable {
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& r, double rayTMin, double rayTMax, HitRecord& rec) const = 0;
};

#endif