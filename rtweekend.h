#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>

// C++ std usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions
inline double DegreesToRadians(double degrees) {
	return degrees * pi / 180.0;
}

inline double RandomDouble() {
	return std::rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max) {
	return min + (max - min) * RandomDouble();
}

// Common headers
#include "color.h"
#include "ray.h"
#include <DirectXMath.h>
#include "interval.h"