#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include <DirectXMath.h>
#include <cmath>

class Sphere : public Hittable {
private:
	XMFLOAT3 center;
	float radius;

public:
	Sphere(const XMFLOAT3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

	bool Hit(const Ray& r, double rayTMin, double rayTMax, HitRecord& rec) const override {
		XMVECTOR dirVec = XMLoadFloat3(&r.GetDirection());

		XMVECTOR oc = XMVectorSet(center.x - r.GetOrigin().x,
			center.y - r.GetOrigin().y,
			center.z - r.GetOrigin().z, 0);

		XMVECTOR a = XMVector3LengthSq(dirVec);

		XMVECTOR h = XMVector3Dot(dirVec, oc);

		XMVECTOR c = XMVector3LengthSq(oc) - XMLoadFloat(&radius) * XMLoadFloat(&radius);

		float discriminant;
		XMVECTOR discriminantVec = XMVectorMultiply(h, h) - XMVectorMultiply(a, c);
		XMStoreFloat(&discriminant, discriminantVec);

		if (discriminant < 0) {
			return false;
		}
		
		XMVECTOR sqrtd = XMVectorSqrt(discriminantVec);

		// Find nearest root in acceptable range
		float root;
		XMVECTOR rootVec = (h - sqrtd) / a;
		XMStoreFloat(&root, rootVec);

		if (root <= rayTMin || rayTMax <= root) {
			rootVec = (h + sqrtd) / a;
			XMStoreFloat(&root, rootVec);
			if (root <= rayTMin || rayTMax <= root)
				return false;
		}

		rec.t = root;
		rec.p = r.At(rec.t);

		XMVECTOR normalVec = (XMVectorSet(rec.p.x, rec.p.y, rec.p.z, 0)
			- XMVectorSet(center.x, center.y, center.z, 0)) / radius;

		XMStoreFloat3(&rec.normal, normalVec);

		return true;
	}

};

#endif