#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

using namespace DirectX;

int main() {
	// World
	HittableList world;
	world.Add(make_shared<Sphere>(XMFLOAT3(0, 0, -1), 0.5));
	world.Add(make_shared<Sphere>(XMFLOAT3(0, -100.5, -1), 100));

	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplesPerPixel = 100;

	cam.Render(world);
}