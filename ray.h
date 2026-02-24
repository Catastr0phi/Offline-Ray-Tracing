#pragma once
#ifndef RAY_H
#define RAY_H

#include <DirectXMath.h>

using namespace DirectX;

class Ray {
private:
	XMFLOAT3 orig;
	XMFLOAT3 dir;

public:
	Ray() {}

	Ray(const XMFLOAT3& origin, const XMFLOAT3& direction) :
		orig(origin),
		dir(direction) {

	}

	const XMFLOAT3& GetOrigin() const { return orig; }
	const XMFLOAT3& GetDirection() const { return dir; }

	XMFLOAT3 At(double t) const {
		XMFLOAT3 point;

		XMVECTOR pointVector = XMLoadFloat3(&orig) + t * XMLoadFloat3(&dir);

		XMStoreFloat3(&point, pointVector);

		return point;
	}
};




#endif