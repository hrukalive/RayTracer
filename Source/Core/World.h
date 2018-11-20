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
    BBox boundingBox;
    std::shared_ptr<Light> ambientLightPtr = nullptr;
	RGBColor backgroundColor = BLACK;
	std::vector<std::shared_ptr<GeometricObject>> objects;
	std::vector<std::shared_ptr<Light>> lights;
public:
    std::shared_ptr<Light>& GetAmbientLightPtr();
    RGBColor& GetBackgroundColor();
    std::vector<std::shared_ptr<GeometricObject>>& GetObjects();
    std::vector<std::shared_ptr<Light>>& GetLights();

    void SetAmbient(std::shared_ptr<Light>& ambient);
    void AddLight(std::shared_ptr<Light>& light);
    void AddObject(std::shared_ptr<GeometricObject>& obj);
    BBox GetBoundingBox();
    FP_TYPE TotalLightPower();

    HitRecord HitObjects(const Ray& ray);
};
