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

	shared_ptr<Material> groundMat = make_shared<Lambertian>(XMFLOAT3(0.8, 0.8, 0.0));
	shared_ptr<Material> centerMat = make_shared<Lambertian>(XMFLOAT3(0.8, 0.0, 0.5));
	shared_ptr<Material> leftMat = make_shared<Dielectric>(1.5);
	shared_ptr<Material> bubbleMat = make_shared<Dielectric>(1.0/1.5);
	shared_ptr<Material> rightMat = make_shared<Metal>(XMFLOAT3(0.8, 0.6, 0.2), 0.3);

	world.Add(make_shared<Sphere>(XMFLOAT3(0, -100.5, -1), 100, groundMat));
	world.Add(make_shared<Sphere>(XMFLOAT3(0, 0, -1.2), 0.5, centerMat));
	world.Add(make_shared<Sphere>(XMFLOAT3(-1, 0, -1), 0.5, leftMat));
	world.Add(make_shared<Sphere>(XMFLOAT3(-1, 0, -1), 0.4, bubbleMat));
	world.Add(make_shared<Sphere>(XMFLOAT3(1, 0, -1), 0.5, rightMat));

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

	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 800;
	cam.samplesPerPixel = 30;
	cam.maxDepth = 50;

	cam.Render(world);
}