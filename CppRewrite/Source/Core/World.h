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
	ViewPlane viewPlane;
    std::shared_ptr<Light> ambientLightPtr = nullptr;
	RGBColor backgroundColor;
	std::vector<GeometricObject*> objects;
	std::vector<Light*> lights;
public:
	ViewPlane& GetViewPlane() { return viewPlane; }
    std::shared_ptr<Light>& GetAmbientLightPtr() { return ambientLightPtr; }
	RGBColor& GetBackgroundColor() { return backgroundColor; }
    std::vector<GeometricObject*>& GetObjects() { return objects; }
    std::vector<Light*>& GetLights() { return lights; }
	HitRecord HitObjects(const Ray& ray);
};
