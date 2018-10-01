/*
  ==============================================================================

    Utility.h
    Created: 28 Sep 2018 11:07:40am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#define FP_TYPE double
#define kEpsilon 0.001
#define PI 3.14159265359
#define INV_PI 0.31830988618

typedef Vector3D<FP_TYPE> Vec3D;
typedef Vec3D Point3D;
typedef Vec3D RGBColor;

struct Ray
{
	Point3D Origin;
	Vec3D Direction;
	Ray() {}
    Ray(Vec3D origin, Vec3D direction) : Origin(origin), Direction(direction) {}
	Point3D GetPoint(FP_TYPE t) const
	{
		return Origin + (Direction * t);
	}
};

class Material;
struct HitRecord
{
	bool Hit = false;
	Vec3D Normal;
	Vec3D HitPoint;
	FP_TYPE T = 0.0f;
	std::shared_ptr<Material> MaterialPtr;
	Ray Ray;
	Vec3D Dir;

	HitRecord() {}
	HitRecord(Vec3D normal, Vec3D hitPoint, FP_TYPE t)
		: Normal(normal), HitPoint(hitPoint), T(t) {}
};
