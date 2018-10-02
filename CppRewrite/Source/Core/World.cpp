/*
  ==============================================================================

    World.cpp
    Created: 28 Sep 2018 11:07:29am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "World.h"
#include <climits>

HitRecord World::HitObjects(const Ray& ray)
{
	HitRecord record;
	FP_TYPE tmin = INFINITY;

	for (int i = 0; i < objects.size(); i++)
	{
		HitRecord tmp = objects[i]->Hit(ray);
		if (tmp.Hit && tmp.T < tmin)
		{
			tmin = tmp.T;
			record = tmp;
            record.WorldPtr.reset(this);
		}
	}
	return record;
}
