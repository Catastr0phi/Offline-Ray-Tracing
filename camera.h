#pragma once

#include "hittable.h"
#include "material.h"

class Camera {
private:
	int imageHeight;
	double pixelSamplesScale;
	XMFLOAT3 camCenter;
	XMFLOAT3 pixel100Loc;
	XMFLOAT3 pixelDeltaU;
	XMFLOAT3 pixelDeltaV;

	void Initialize() {
		// Calculate height
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		pixelSamplesScale = 1.0 / samplesPerPixel;

		// Camera
		camCenter = XMFLOAT3(0, 0, 0);
		double focalLength = 1.0;
		double viewportHeight = 2.0;
		double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

		// Calculate vectors across viewport edges
		XMFLOAT3 viewportU = XMFLOAT3(viewportWidth, 0, 0);
		XMFLOAT3 viewportV = XMFLOAT3(0, -viewportHeight, 0);

		// Calculate pixel to pixel delta vectors
		pixelDeltaU = XMFLOAT3(viewportWidth / imageWidth, 0, 0);
		pixelDeltaV = XMFLOAT3(0, -viewportHeight / imageHeight, 0);

		// Calculate location of upper left pixel
		XMFLOAT3 viewportUpperLeft;
		XMVECTOR viewportUpperLeftVector = XMLoadFloat3(&camCenter)
			- XMVectorSet(0, 0, focalLength, 0)
			- XMLoadFloat3(&viewportU) / 2
			- XMLoadFloat3(&viewportV) / 2;
		XMStoreFloat3(&viewportUpperLeft, viewportUpperLeftVector);

		pixel100Loc;
		XMVECTOR pixel100LocVector = viewportUpperLeftVector
			+ 0.5 * (XMLoadFloat3(&pixelDeltaU) + XMLoadFloat3(&pixelDeltaV));
		XMStoreFloat3(&pixel100Loc, pixel100LocVector);
	}

	Ray GetRay(int i, int j) const {
		// Construct a camera ray originating from the origin and directed at randomly sampled
		// point around the pixel location i, j.

		XMFLOAT3 offset = SampleSquare();

		XMFLOAT3 pixelSample;
		XMVECTOR pixelSampleVec = XMLoadFloat3(&pixel100Loc)
			+ ((i + offset.x)) * XMLoadFloat3(&pixelDeltaU)
			+ ((j + offset.y)) * XMLoadFloat3(&pixelDeltaV);

		XMStoreFloat3(&pixelSample, pixelSampleVec);
	
		XMFLOAT3 rayOrigin = camCenter;
		XMFLOAT3 rayDirection = XMFLOAT3(
			pixelSample.x - rayOrigin.x,
			pixelSample.y - rayOrigin.y,
			pixelSample.z - rayOrigin.z
		);

		return Ray(rayOrigin, rayDirection);
	}

	XMFLOAT3 SampleSquare() const {
		return XMFLOAT3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
	}

	color RayColor(const Ray& r, int depth, const Hittable& world) const {
		if (depth <= 0) {
			return XMFLOAT3(0, 0, 0);
		}
		
		HitRecord rec;
		if (world.Hit(r, Interval(0.001, infinity), rec)) {
			Ray scattered;
			XMFLOAT3 attenuation;
			if (rec.mat->Scatter(r, rec, attenuation, scattered)) {
				XMFLOAT3 currentColor = RayColor(scattered, depth - 1, world);
				XMFLOAT3 attenuated;
				XMVECTOR attenuatedVec = XMVectorMultiply(XMLoadFloat3(&attenuation), XMLoadFloat3(&currentColor));
				XMStoreFloat3(&attenuated, attenuatedVec);
				return attenuated;
			}
			return color(0, 0, 0);
		}

		XMFLOAT3 unitDir;
		XMVECTOR unitDirVector = XMLoadFloat3(&r.GetDirection());
		unitDirVector = XMVector3Normalize(unitDirVector);
		XMStoreFloat3(&unitDir, unitDirVector);

		double a = 0.5 * (unitDir.y + 1.0);
		return color((1.0 - a) * 1.0 + a * 0.5,
			(1.0 - a) * 1.0 + a * 0.7,
			(1.0 - a) * 1.0 + a * 1.0);
	}

public: 
	double aspectRatio = 1.0;
	int imageWidth = 100;
	int samplesPerPixel = 10;
	int maxDepth = 10;

	void Render(const Hittable& world) {
		Initialize();

		// render
		std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

		for (int j = 0; j < imageHeight; j++) {
			std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
			for (int i = 0; i < imageWidth; i++) {
				XMFLOAT3 pixelColor = XMFLOAT3(0, 0, 0);
				for (int sample = 0; sample < samplesPerPixel; sample++) {
					Ray r = GetRay(i, j);
					XMFLOAT3 rayColor = RayColor(r, maxDepth, world);
					pixelColor = XMFLOAT3(
						pixelColor.x + rayColor.x,
						pixelColor.y + rayColor.y,
						pixelColor.z + rayColor.z
					);
				}

				XMStoreFloat3(&pixelColor, XMLoadFloat3(&pixelColor) * pixelSamplesScale);
				WriteColor(std::cout, pixelColor);
			}
		}

		std::clog << "\rDone.                                     \n";
	}
};