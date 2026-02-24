#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

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

	bool Hit(const Ray& r, double rayTMin, double rayTMax, HitRecord& rec) const override {
		HitRecord tempRec;

		bool hitAnything = false;

		double closest = rayTMax;

		for (const shared_ptr<Hittable>& object : objects) {
			if (object->Hit(r, rayTMin, closest, tempRec)) {
				hitAnything = true;
				closest = tempRec.t;
				rec = tempRec;
			}
		}

		return hitAnything;
	}
};

#endif