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
	Camera camera;
	Light ambientLight;
	RGBColor backgroundColor;
	std::vector<GeometricObject*> objects;
	std::vector<Light*> lights;
public:
	ViewPlane& GetViewPlane() { return viewPlane; }
	RGBColor& GetBackgroundColor() { return backgroundColor; }
	HitRecord HitObjects(const Ray& ray);
};
