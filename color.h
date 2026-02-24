#pragma once

#include "rtweekend.h"
#include <DirectXMath.h>

using color = DirectX::XMFLOAT3;

void WriteColor(std::ostream& out, const color& pixelColor) {
	float r = pixelColor.x;
	float g = pixelColor.y;
	float b = pixelColor.z;

	// Translate [0,1] components to [0,255]
	int rbyte = int(255.999 * r);
	int gbyte = int(255.999 * g);
	int bbyte = int(255.999 * b);

	// Write out pixel color
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}