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
typedef Point<FP_TYPE> Point2D;

#define BLACK (RGBColor(0.0, 0.0, 0.0))
#define WHITE (RGBColor(1.0, 1.0, 1.0))
#define RED (RGBColor(1.0, 0.0, 0.0))

//std::ostream& operator<<(std::ostream& o, const Vec3D& vec)
//{
//    o << vec.x << ", " << vec.y << ", " << vec.z;
//    return o;
//}

inline Vec3D ElemMul(const Vec3D& a, const Vec3D& b)
{
    return Vec3D(a.x * b.x, a.y * b.y, a.z * b.z);
}

struct Ray
{
	Point3D Origin;
	Vec3D Direction;
	Ray() {}
    Ray(Vec3D origin, Vec3D direction) : Origin(origin), Direction(direction) {}
	inline Point3D GetPoint(FP_TYPE t) const
	{
		return Origin + (Direction * t);
	}
};

class Material;
class World;
struct HitRecord
{
	bool Hit = false;
	Vec3D Normal;
	Vec3D HitPoint;
	FP_TYPE T = 0.0f;
	std::shared_ptr<Material> MaterialPtr = nullptr;
	Ray Ray;
	Vec3D Dir;
    std::shared_ptr<World> WorldPtr = nullptr;

	HitRecord() {}
	HitRecord(Vec3D normal, Vec3D hitPoint, FP_TYPE t)
		: Normal(normal), HitPoint(hitPoint), T(t) {}
};
