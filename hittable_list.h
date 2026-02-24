#pragma once

#include "hittable.h"
#include "rtweekend.h"
#include <vector>

class HittableList : public Hittable {
public:
	std::vector<shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(shared_ptr<Hittable> object) { Add(object); }

	void Clear() { objects.clear(); }

	void Add(shared_ptr<Hittable> object) {
		objects.push_back(object);
	}

	bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
		HitRecord tempRec;

		bool hitAnything = false;

		double closest = rayT.max;

		for (const shared_ptr<Hittable>& object : objects) {
			if (object->Hit(r,Interval(rayT.min, closest), tempRec)) {
				hitAnything = true;
				closest = tempRec.t;
				rec = tempRec;
			}
		}

		return hitAnything;
	}
};