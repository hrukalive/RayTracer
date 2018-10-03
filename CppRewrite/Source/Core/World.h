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
	RGBColor backgroundColor;
	std::vector<GeometricObject*> objects;
	std::vector<Light*> lights;
public:
    std::shared_ptr<Light>& GetAmbientLightPtr() { return ambientLightPtr; }
	RGBColor& GetBackgroundColor() { return backgroundColor; }
    std::vector<GeometricObject*>& GetObjects() { return objects; }
    std::vector<Light*>& GetLights() { return lights; }
    void AddLight(Light* light) { lights.push_back(light); }
    void AddObject(GeometricObject* obj) { objects.push_back(obj); }
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
                record.WorldPtr.reset(this);
            }
        }
        return record;
    }
};
