/*
  ==============================================================================

    GeometricObject.h
    Created: 28 Sep 2018 11:00:02am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../Utility.h"

class GeometricObject
{
protected:
    std::shared_ptr<Material> materialPtr;
public:
    virtual ~GeometricObject() {}
    void SetMaterial(std::shared_ptr<Material>& materialPtr) { this->materialPtr = materialPtr; }
	virtual HitRecord Hit(const Ray& ray) = 0;
};

#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"