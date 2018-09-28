/*
  ==============================================================================

    Plane.h
    Created: 28 Sep 2018 11:00:58am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../Utility.h"
#include "GeometricObject.h"

class InfPlane : public GeometricObject
{
	Vector3D<FP_TYPE> a;
	Vector3D<FP_TYPE> n;
public:
	InfPlane(Vector3D<FP_TYPE> origin, Vector3D<FP_TYPE> normal) : a(origin), n(normal) {}
	void setOrigin(Vector3D<FP_TYPE> origin) { a = origin; }
	void setNormal(Vector3D<FP_TYPE> normal) { n = normal; }
	Vector3D<FP_TYPE> getOrigin() { return a; }
	Vector3D<FP_TYPE> getNormal() { return n; }

	virtual bool Hit(Ray& ray, HitRecord& record)
	{
		FP_TYPE t = ((a - ray.Origin) * n) / (ray.Direction * n);
		if (t > kEpsilon)
		{
			record.Normal = n.normalised();
			record.Position = ray.GetPoint(t);
			record.T = t;
			return true;
		}
		return false;
	}
};

class Plane : public GeometricObject
{
	Vector3D<FP_TYPE> a;
	Vector3D<FP_TYPE> u;
	Vector3D<FP_TYPE> v;
	Vector3D<FP_TYPE> n;
public:
	Plane(Vector3D<FP_TYPE> origin, Vector3D<FP_TYPE> uDirection, Vector3D<FP_TYPE> vDirection)
		: a(origin), u(uDirection), v(vDirection)
	{
		n = u ^ v;
	}

	void setOrigin(Vector3D<FP_TYPE> origin) { a = origin; }
	void setU(Vector3D<FP_TYPE> uDirection) { u = uDirection; n = u ^ v; }
	void setV(Vector3D<FP_TYPE> vDirection) { v = vDirection; n = u ^ v; }
	Vector3D<FP_TYPE> getOrigin() { return a; }
	Vector3D<FP_TYPE> getU() { return u; }
	Vector3D<FP_TYPE> getV() { return v; }

	virtual bool Hit(Ray& ray, HitRecord& record)
	{
		FP_TYPE t = ((a - ray.Origin) * n) / (ray.Direction * n);
		auto p = ray.GetPoint(t);
		auto tu = (p - a) * u.normalised() / u.length();
		auto tv = (p - a) * v.normalised() / v.length();

		if (t > kEpsilon && tu >= 0.0 && tu <= 1.0 && tv >= 0.0 && tv <= 1.0)
		{
			record.Normal = n.normalised();
			record.Position = p;
			record.T = t;
			return true;
		}
		return false;
	}
};
