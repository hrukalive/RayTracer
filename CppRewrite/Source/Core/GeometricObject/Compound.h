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
protected:
	std::vector<std::shared_ptr<GeometricObject>> objects;
	void UpdateBoundingBox()
	{
		if (objects.size() == 1)
		{
			boundingBox = objects[0]->GetBoundingBox();
		}
		else if(objects.size() > 1)
		{
			boundingBox.Merge(objects[objects.size() - 1]->GetBoundingBox());
		}
	}
public:
	virtual ~Compound() {}
	void AddObject(std::shared_ptr<GeometricObject>& obj) 
	{ 
		objects.push_back(obj); 
		UpdateBoundingBox(); 
	}
	void SetMaterial(std::shared_ptr<Material>& material)
	{
		for (auto& obj : objects)
		{
			obj->SetMaterial(material);
		}
	}
	HitRecord Hit(const Ray& ray) override
	{
		HitRecord record;
		FP_TYPE tmin = INFINITY;

		if (!boundingBox.Hit(ray).Hit)
		{
			return record;
		}

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
