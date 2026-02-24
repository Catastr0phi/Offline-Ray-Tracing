#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class HitRecord {
public:
	XMFLOAT3 p;
	XMFLOAT3 normal;
	double t;
	bool frontFace;

	// Sets hit record normal vector
	void SetFaceNormal(const Ray& r, const XMFLOAT3& outwardNormal) {
		float dotProduct;
		XMVECTOR dotProductVec = XMVector3Dot(XMLoadFloat3(&r.GetDirection()), XMLoadFloat3(&outwardNormal));
		XMStoreFloat(&dotProduct, dotProductVec);

		frontFace = dotProduct < 0;
		normal = frontFace ? outwardNormal : XMFLOAT3(-outwardNormal.x,-outwardNormal.y,-outwardNormal.z);
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& r, double rayTMin, double rayTMax, HitRecord& rec) const = 0;
};

#endif