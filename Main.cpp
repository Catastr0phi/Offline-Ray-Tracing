#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

using namespace DirectX;

int main() {
	// World
	HittableList world;

	//shared_ptr<Material> groundMat = make_shared<Lambertian>(XMFLOAT3(0.8, 0.8, 0.0));
	//shared_ptr<Material> centerMat = make_shared<Lambertian>(XMFLOAT3(0.8, 0.0, 0.5));
	//shared_ptr<Material> leftMat = make_shared<Dielectric>(1.5);
	//shared_ptr<Material> bubbleMat = make_shared<Dielectric>(1.0/1.5);
	//shared_ptr<Material> rightMat = make_shared<Metal>(XMFLOAT3(0.8, 0.6, 0.2), 0.3);

	//world.Add(make_shared<Sphere>(XMFLOAT3(0, -100.5, -1), 100, groundMat));
	//world.Add(make_shared<Sphere>(XMFLOAT3(0, 0, -1.2), 0.5, centerMat));
	//world.Add(make_shared<Sphere>(XMFLOAT3(-1, 0, -1), 0.5, leftMat));
	//world.Add(make_shared<Sphere>(XMFLOAT3(-1, 0, -1), 0.4, bubbleMat));
	//world.Add(make_shared<Sphere>(XMFLOAT3(1, 0, -1), 0.5, rightMat));

	//for (int i = 0; i < 10; i++) {
	//	shared_ptr<Material> currentMat;
	//	if (RandomDouble() <= 0.5) currentMat = centerMat;
	//	else currentMat = leftMat;

	//	world.Add(make_shared<Sphere>(XMFLOAT3(
	//		RandomDouble(-1,1),
	//		RandomDouble(-1,1),
	//		RandomDouble(-2,-1)
	//	), RandomDouble(0.05, 0.1), currentMat));
	//}

	//for (float i = 0.0; i < 11.0; i++) {
	//	shared_ptr<Material> fuzzMat = make_shared<Metal>(XMFLOAT3(0.8, 0.8, 0.8), i/10.0);

	//	world.Add(make_shared<Sphere>(XMFLOAT3((i / 5.0) - 1.0, 0, -0.6), 0.09, fuzzMat));
	//}

	shared_ptr<Material> groundMat = make_shared<Lambertian>(XMFLOAT3(0.5, 0.5, 0.5));
	world.Add(make_shared<Sphere>(XMFLOAT3(0, -1000, 0), 1000, groundMat));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double chooseMat = RandomDouble();
			XMFLOAT3 center = XMFLOAT3(a + 0.9*RandomDouble(), 0.2, b+0.9*RandomDouble());
			XMVECTOR centerVec = XMLoadFloat3(&center);
			float distance;
			XMVECTOR distanceVec = XMVector3Length(centerVec - XMVectorSet(4,0.2,0,0));
			XMStoreFloat(&distance, distanceVec);

			if (distance > 0.9) {
				shared_ptr<Material> sphereMat;

				if (chooseMat < 0.8) {
					// Diffuse
					XMFLOAT3 albedo;
					XMFLOAT3 rand1 = RandomFloat3();
					XMFLOAT3 rand2 = RandomFloat3();

					XMStoreFloat3(&albedo,
						XMLoadFloat3(&rand1) * 
						XMLoadFloat3(&rand2));
					sphereMat = make_shared<Lambertian>(albedo);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMat));
				}
				else if (chooseMat < 0.95) {
					// Metal
					XMFLOAT3 albedo = RandomFloat3(0.5,1);
					double fuzz = RandomDouble(0, 0.5);

					sphereMat = make_shared<Metal>(albedo, fuzz);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMat));
				}
				else {
					// Glass
					sphereMat = make_shared<Dielectric>(1.5);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMat));
				}
			}
		}
	}

	shared_ptr<Material> mat1 = make_shared<Dielectric>(1.5);
	world.Add(make_shared<Sphere>(XMFLOAT3(0, 1, 0), 1.0, mat1));

	shared_ptr<Material> mat2 = make_shared<Lambertian>(XMFLOAT3(0.4,0.2,0.1));
	world.Add(make_shared<Sphere>(XMFLOAT3(-4, 1, 0), 1.0, mat2));

	shared_ptr<Material> mat3 = make_shared<Metal>(XMFLOAT3(0.7,0.6,0.5), 0.0);
	world.Add(make_shared<Sphere>(XMFLOAT3(4, 1, 0), 1.0, mat3));

	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplesPerPixel = 10;
	cam.maxDepth = 50;

	cam.vfov = 20;
	cam.lookFrom = XMFLOAT3(13, 2, 3);
	cam.lookAt = XMFLOAT3(0, 0, 0);
	cam.vup = XMFLOAT3(0, 1, 0);

	cam.defocusAngle = 0.6;
	cam.focusDist = 10.0;

	cam.Render(world);
}