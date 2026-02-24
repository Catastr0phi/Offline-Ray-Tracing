#include <iostream>
#include "color.h"
#include <DirectXMath.h>
#include "ray.h"

using namespace DirectX;

double HitSphere(const XMFLOAT3& center, float radius, const ray& r) {
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
		return -1.0;
	}
	else {
		float at;
		XMVECTOR atVec = (h - XMVectorSqrt(discriminantVec)) / a;
		XMStoreFloat(&at, atVec);

		return at;
	}
}

color RayColor(const ray& r) {
	double t = (HitSphere(XMFLOAT3(0, 0, -1), 0.5, r));

	if (t > 0.0) {
		XMFLOAT3 finalColor;
		XMVECTOR N = XMVectorSet(r.At(t).x, r.At(t).y, r.At(t).z,0);
		N = N - XMVectorSet(0, 0, -1, 0);
		N = 0.5 * (N + XMVectorSet(1, 1, 1, 0));
		XMStoreFloat3(&finalColor, N);
		return finalColor;

	}

	XMFLOAT3 unitDir;
	XMVECTOR unitDirVector = XMLoadFloat3(&r.GetDirection());
	unitDirVector = XMVector3Normalize(unitDirVector);
	XMStoreFloat3(&unitDir, unitDirVector);

	double a = 0.5 * (unitDir.y + 1.0);
	return color((1.0-a)*1.0 + a*0.5,
		(1.0 - a) * 1.0 + a * 0.7,
		(1.0 - a) * 1.0 + a * 1.0);
}

int main() {
	double aspectRatio = 16.0 / 9.0;
	int imageWidth = 400;

	// Calculate height
	int imageHeight = int(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// Camera
	double focalLength = 1.0;
	double viewportHeight = 2.0;
	double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
	XMFLOAT3 camCenter = XMFLOAT3(0, 0, 0);

	// Calculate vectors across viewport edges
	XMFLOAT3 viewportU = XMFLOAT3(viewportWidth, 0, 0);
	XMFLOAT3 viewportV = XMFLOAT3(0, -viewportHeight, 0);

	// Calculate pixel to pixel delta vectors
	XMFLOAT3 pixelDeltaU = XMFLOAT3(viewportWidth/imageWidth, 0, 0);
	XMFLOAT3 pixelDeltaV = XMFLOAT3(0, -viewportHeight/imageHeight, 0);

	// Calculate location of upper left pixel
	XMFLOAT3 viewportUpperLeft;
	XMVECTOR viewportUpperLeftVector = XMLoadFloat3(&camCenter) 
		- XMVectorSet(0,0,focalLength,0) 
		- XMLoadFloat3(&viewportU)/2
		- XMLoadFloat3(&viewportV)/2;
	XMStoreFloat3(&viewportUpperLeft, viewportUpperLeftVector);

	XMFLOAT3 pixel100Loc;
	XMVECTOR pixel100LocVector = viewportUpperLeftVector
		+ 0.5 * (XMLoadFloat3(&pixelDeltaU) + XMLoadFloat3(&pixelDeltaV));
	XMStoreFloat3(&pixel100Loc, pixel100LocVector);

	// render
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	for (int j = 0; j < imageHeight; j++) {
		std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
		for (int i = 0; i < imageWidth; i++) {
			XMFLOAT3 pixelCenter;
			XMVECTOR pixelCenterVector = pixel100LocVector + (i * XMLoadFloat3(&pixelDeltaU)) + (j * XMLoadFloat3(&pixelDeltaV));
			XMStoreFloat3(&pixelCenter, pixelCenterVector);

			XMFLOAT3 rayDir = XMFLOAT3(pixelCenter.x - camCenter.x, pixelCenter.y - camCenter.y, pixelCenter.z - camCenter.z);
			ray r(camCenter, rayDir);

			color pixelColor = RayColor(r);

			WriteColor(std::cout, pixelColor);
		}
	}

	std::clog << "\rDone.                                     \n";
}