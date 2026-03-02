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
	//world.Add(make_shared<Sphere>(XMFLOAT3(0, -1000, 0), 1000, groundMat));

	for (int a = -20; a <= 20; a++) {
		for (int b = -20; b <= 20; b++) {
			double chooseMat = BetterRandom();
			XMFLOAT3 center = XMFLOAT3(a/4.0 + 0.9*BetterRandom(), BetterRandom(0,10), b/4.0 + 0.9 * BetterRandom());
			XMVECTOR centerVec = XMLoadFloat3(&center);
			float distance;
			XMVECTOR distanceVec = XMVector3Length(centerVec - XMVectorSet(0,5,0,0));
			XMStoreFloat(&distance, distanceVec);

			if (distance > 4) {
				shared_ptr<Material> sphereMat;

				if (chooseMat < 0.6) {
					// Diffuse
					float val = BetterRandom();
					XMFLOAT3 albedo;
					XMFLOAT3 rand1 = RandomFloat3();
					XMFLOAT3 rand2 = RandomFloat3();

					XMStoreFloat3(&albedo,
						XMLoadFloat3(&rand1) * 
						XMLoadFloat3(&rand2));
					sphereMat = make_shared<Lambertian>(albedo);
					world.Add(make_shared<Sphere>(center, 0.1, sphereMat));
				}
				else if (chooseMat < 0.9) {
					// Metal
					XMFLOAT3 albedo = RandomFloat3(0.5,1);
					double fuzz = BetterRandom(0, 0.5);

					sphereMat = make_shared<Metal>(albedo, fuzz);
					world.Add(make_shared<Sphere>(center, 0.1, sphereMat));
				}
				else {
					// Glass
					sphereMat = make_shared<Dielectric>(1.5);
					world.Add(make_shared<Sphere>(center, 0.1, sphereMat));
				}
			}
			else {
				b--;
			}
		}
	}

	//shared_ptr<Material> mat1 = make_shared<Dielectric>(1.5);
	//world.Add(make_shared<Sphere>(XMFLOAT3(0, 5, 0), 1.0, mat1));

	//shared_ptr<Material> mat2 = make_shared<Lambertian>(XMFLOAT3(0.4,0.2,0.1));
	//world.Add(make_shared<Sphere>(XMFLOAT3(-4, 1, 0), 1.0, mat2));

	shared_ptr<Material> mat3 = make_shared<Metal>(XMFLOAT3(0.5,0.4,1.0), 0.0);
	world.Add(make_shared<Sphere>(XMFLOAT3(0, 5, 0), 2.0, mat3));

	Camera cam;

	cam.aspectRatio = 1;
	cam.imageWidth = 400;
	cam.samplesPerPixel = 20;
	cam.maxDepth = 50;

	cam.vfov = 40;
	cam.lookFrom = XMFLOAT3(10, 2, 10);
	cam.lookAt = XMFLOAT3(0, 5, 0);
	cam.vup = XMFLOAT3(0, 1, 0);

	cam.defocusAngle = 0.6;
	cam.focusDist = 10.0;

	cam.Render(world);
}