#pragma once
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

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

// Common headers
#include "color.h"
#include "ray.h"
#include <DirectXMath.h>

#endif