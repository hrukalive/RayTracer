/*
  ==============================================================================

    World.h
    Created: 28 Sep 2018 11:07:29am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "Utility.h"
#include "GeometricObject/GeometricObject.h"
#include "Viewplane.h"
#include "Camera.h"
#include "Light.h"

class World
{
    std::shared_ptr<Light> ambientLightPtr = nullptr;
	RGBColor backgroundColor = BLACK;
	std::vector<std::shared_ptr<GeometricObject>> objects;
	std::vector<std::shared_ptr<Light>> lights;
public:
    std::shared_ptr<Light>& GetAmbientLightPtr() { return ambientLightPtr; }
	RGBColor& GetBackgroundColor() { return backgroundColor; }
    auto& GetObjects() { return objects; }
	auto& GetLights() { return lights; }
	void SetAmbient(std::shared_ptr<Light>& ambient) { ambientLightPtr = ambient; }
    void AddLight(std::shared_ptr<Light>& light) { lights.push_back(light); }
    void AddObject(std::shared_ptr<GeometricObject>& obj) { objects.push_back(obj); DBG(objects.size()); }
	HitRecord HitObjects(const Ray& ray)
    {
        HitRecord record;
        FP_TYPE tmin = INFINITY;
        
        for (int i = 0; i < objects.size(); i++)
        {
            HitRecord tmp = objects[i]->Hit(ray);
            if (tmp.Hit && tmp.T < tmin)
            {
                DBG("HIT");
                tmin = tmp.T;
                record = tmp;
                record.WorldPtr.reset(this);
            }
        }
        return record;
    }
};
