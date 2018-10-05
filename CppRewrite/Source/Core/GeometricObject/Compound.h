/*
  ==============================================================================

    Compound.h
    Created: 4 Oct 2018 4:04:19pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"
#include "BBox.h"

class Compound : public GeometricObject
{
	std::vector<std::shared_ptr<GeometricObject>> objects;
public:
    Compound() = default;
	void AddObject(std::shared_ptr<GeometricObject>& obj) { objects.push_back(obj); }
	void SetMaterial(std::shared_ptr<Material>& material)
	{
		for (auto& obj : objects)
		{
			obj->SetMaterial(material);
		}
	}
	HitRecord HitObjects(const Ray& ray)
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
			}
		}
		return record;
	}
};
