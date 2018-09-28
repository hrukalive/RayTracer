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
public:
	virtual bool Hit(Ray& ray, HitRecord& record) = 0;
};

#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"