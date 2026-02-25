#pragma once

#include "hittable.h"
#include "rtweekend.h"

class Material {
public:
	virtual ~Material() = default;

	virtual bool Scatter(
		const Ray& rIn, const HitRecord& rec, XMFLOAT3& attenuation, Ray& scattered
	) const {
		return false;
	}
};

class Lambertian : public Material {
private:
	XMFLOAT3 albedo;
public:
	Lambertian(const XMFLOAT3& albedo) : albedo(albedo) {}

	bool Scatter(const Ray& rIn, const HitRecord& rec, XMFLOAT3& attenuation, Ray& scattered)
		const override {
		XMFLOAT3 randomVector = RandomUnitVector();
		XMFLOAT3 direction = XMFLOAT3(
			rec.normal.x + randomVector.x,
			rec.normal.y + randomVector.y,
			rec.normal.z + randomVector.z);

		// Catch directions that are near zero to prevent issues
		if (NearZero(direction)) {
			direction = rec.normal;
		}

		scattered = Ray(rec.p, direction);
		attenuation = albedo;
		return true;
	}
};

class Metal : public Material {
private:
	XMFLOAT3 albedo;
	double fuzz;
public:
	Metal(const XMFLOAT3& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool Scatter(const Ray& rIn, const HitRecord& rec, XMFLOAT3& attenuation, Ray& scattered)
		const override {
		XMFLOAT3 reflected;
		XMVECTOR reflectedVec = XMVector3Reflect(XMLoadFloat3(&rIn.GetDirection()), XMLoadFloat3(&rec.normal));
		reflectedVec = XMVector3Normalize(reflectedVec);
		XMStoreFloat3(&reflected, reflectedVec);
		XMFLOAT3 fuzzVector = RandomUnitVector();

		reflected = XMFLOAT3(
			reflected.x + fuzz * fuzzVector.x,
			reflected.y + fuzz * fuzzVector.y,
			reflected.z + fuzz * fuzzVector.z
		);

		scattered = Ray(rec.p, reflected);
		attenuation = albedo;

		float dot;
		XMVECTOR dotVec = XMVector3Dot(XMLoadFloat3(&scattered.GetDirection()), XMLoadFloat3(&rec.normal));
		XMStoreFloat(&dot, dotVec);

		return dot > 0;
	}
};