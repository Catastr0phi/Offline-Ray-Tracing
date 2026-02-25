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

class Dielectric : public Material {
private:
	double refractionIndex;

	static double Reflectance(double cosine, double refractionIndex) {
		// Schlicks approximation
		double r0 = (1 - refractionIndex) / (1 + refractionIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}

public:
	Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

	bool Scatter(const Ray& rIn, const HitRecord& rec, XMFLOAT3& attenuation, Ray& scattered)
		const override {
		attenuation = XMFLOAT3(1.0, 1.0, 1.0);

		double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

		XMVECTOR unitDir = XMVector3Normalize(XMLoadFloat3(&rIn.GetDirection()));
		XMVECTOR normal = XMLoadFloat3(&rec.normal);
		float cosTheta;
		XMStoreFloat(&cosTheta, XMVector3Dot(-unitDir,normal));
		cosTheta = std::fmin(cosTheta, 1.0);
		float sinTheta = std::sqrt(1.0-cosTheta*cosTheta);

		bool cannotRefract = ri * sinTheta > 1.0;

		XMFLOAT3 direction;
		XMVECTOR directionVec;

		if (cannotRefract || Reflectance(cosTheta, ri) > RandomDouble()) directionVec = XMVector3Reflect(unitDir, normal);
		else directionVec = XMVector3Refract(unitDir, normal, ri);

		XMStoreFloat3(&direction, directionVec);

		scattered = Ray(rec.p, direction);
		return true;
	}
};