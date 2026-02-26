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
	XMFLOAT3 u, v, w;
	XMFLOAT3 defocusDiskU;
	XMFLOAT3 defocusDiskV;

	void Initialize() {
		// Calculate height
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		pixelSamplesScale = 1.0 / samplesPerPixel;

		// Get some vectors
		XMVECTOR lookFromVec = XMLoadFloat3(&lookFrom);
		XMVECTOR lookAtVec = XMLoadFloat3(&lookAt);
		XMVECTOR vupVec = XMLoadFloat3(&vup);

		// Camera
		camCenter = lookFrom;
		double theta = DegreesToRadians(vfov);
		double h = std::tan(theta / 2);
		double viewportHeight = 2 * h * focusDist;
		double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

		// Calculate u,v,w vectors
		XMVECTOR wVec = XMVector3Normalize(lookFromVec - lookAtVec);
		XMVECTOR uVec = XMVector3Normalize(XMVector3Cross(vupVec, wVec));
		XMVECTOR vVec = XMVector3Cross(wVec, uVec);
		XMStoreFloat3(&w, wVec);
		XMStoreFloat3(&u, uVec);
		XMStoreFloat3(&v, vVec);

		// Calculate vectors across viewport edges
		XMFLOAT3 viewportU;
		XMFLOAT3 viewportV;
		XMVECTOR viewportUVec = viewportWidth * uVec;
		XMVECTOR viewportVVec = viewportHeight * -vVec;
		XMStoreFloat3(&viewportU, viewportUVec);
		XMStoreFloat3(&viewportV, viewportVVec);

		// Calculate pixel to pixel delta vectors
		pixelDeltaU = XMFLOAT3(
			viewportU.x / imageWidth, 
			viewportU.y / imageWidth,
			viewportU.z / imageWidth);
		pixelDeltaV = XMFLOAT3(
			viewportV.x / imageHeight,
			viewportV.y / imageHeight,
			viewportV.z / imageHeight);

		// Calculate location of upper left pixel
		XMFLOAT3 viewportUpperLeft;
		XMVECTOR viewportUpperLeftVector = XMLoadFloat3(&camCenter)
			- (focusDist * wVec)
			- (viewportUVec / 2)
			- (viewportVVec / 2);
		XMStoreFloat3(&viewportUpperLeft, viewportUpperLeftVector);

		XMVECTOR pixel100LocVector = viewportUpperLeftVector
			+ 0.5 * (XMLoadFloat3(&pixelDeltaU) + XMLoadFloat3(&pixelDeltaV));
		XMStoreFloat3(&pixel100Loc, pixel100LocVector);

		// Calculate camera defocus disk basis vectors
		float defocusRadius = focusDist * std::tan(DegreesToRadians(defocusAngle / 2));
		XMStoreFloat3(&defocusDiskU, uVec * defocusRadius);
		XMStoreFloat3(&defocusDiskV, vVec * defocusRadius);
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
	
		XMFLOAT3 rayOrigin = (defocusAngle <= 0) ? camCenter : DefocusDiskSample();
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

	XMFLOAT3 DefocusDiskSample() const {
		XMFLOAT3 p = RandomInUnitDisk();
		XMFLOAT3 sample;
		XMVECTOR centerVec = XMLoadFloat3(&camCenter);
		XMVECTOR defocusUVec = XMLoadFloat3(&defocusDiskU);
		XMVECTOR defocusVVec = XMLoadFloat3(&defocusDiskV);
		XMStoreFloat3(&sample, centerVec + (p.x * defocusUVec) + (p.y * defocusVVec));
		return sample;
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
		return color((1.0 - a) * 1.0 + a * 0.3,
			(1.0 - a) * 1.0 + a * 0.0,
			(1.0 - a) * 1.0 + a * 0.6);
	}

public: 
	double aspectRatio = 1.0;
	int imageWidth = 100;
	int samplesPerPixel = 10;
	int maxDepth = 10;
	double vfov = 90;
	XMFLOAT3 lookFrom = XMFLOAT3(0, 0, 0);
	XMFLOAT3 lookAt = XMFLOAT3(0, 0, -1);
	XMFLOAT3 vup = XMFLOAT3(0, 1, 0);
	double defocusAngle = 0;
	double focusDist = 10;

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