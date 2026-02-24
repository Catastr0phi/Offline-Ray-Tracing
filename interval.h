#pragma once

#include "rtweekend.h"

class Interval {
public:
	double min, max;

	Interval() : min(+infinity), max(-infinity) {}
	Interval(double min, double max) : min(min), max(max) {}

	double Size() const {
		return max - min;
	}

	bool Contains(double x) const {
		return min <= x && x <= max;
	}

	bool Surrounds(double x) const {
		return min < x && x < max;
	}

	static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);