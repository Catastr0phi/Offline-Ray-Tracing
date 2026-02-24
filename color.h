#pragma once

#include "rtweekend.h"
#include "interval.h"
#include <DirectXMath.h>

using color = DirectX::XMFLOAT3;

inline double LinearToGamma(double linearComponent) {
	if (linearComponent > 0) {
		return std::sqrt(linearComponent);
	}

	return 0;
}

void WriteColor(std::ostream& out, const color& pixelColor) {
	float r = pixelColor.x;
	float g = pixelColor.y;
	float b = pixelColor.z;

	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	// Translate [0,1] components to [0,255]
	static const Interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.Clamp(r));
	int gbyte = int(256 * intensity.Clamp(g));
	int bbyte = int(256 * intensity.Clamp(b));

	// Write out pixel color
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}