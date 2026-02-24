#pragma once
#include <DirectXMath.h>
#include "rtweekend.h"

inline XMFLOAT3 RandomFloat3() {
	return XMFLOAT3(RandomDouble(), RandomDouble(), RandomDouble());
}

inline XMFLOAT3 RandomFloat3(double min, double max) {
	return XMFLOAT3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
}

inline XMFLOAT3 RandomUnitVector() {
	while (true) {
		XMFLOAT3 p = RandomFloat3(-1, 1);
		XMVECTOR pVec = XMLoadFloat3(&p);
		float lensq;
		XMStoreFloat(&lensq, XMVector3LengthSq(pVec));
		if (1e-160 < lensq && lensq <= 1) {
			XMFLOAT3 unit;
			XMStoreFloat3(&unit, pVec / sqrt(lensq));
			return unit;
		}
	}
}

inline XMFLOAT3 RandomOnHemisphere(const XMFLOAT3& normal) {
	XMFLOAT3 onUnitSphere = RandomUnitVector();
	float dot;
	XMStoreFloat(&dot, XMVector3Dot(XMLoadFloat3(&onUnitSphere), XMLoadFloat3(&normal)));
	if (dot > 0.0) {
		return onUnitSphere;
	}
	else {
		return XMFLOAT3(
			-onUnitSphere.x,
			-onUnitSphere.y,
			-onUnitSphere.z);
	}
}