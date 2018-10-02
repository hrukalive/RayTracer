/*
  ==============================================================================

    Triangle.h
    Created: 28 Sep 2018 11:03:10am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../Utility.h"
#include "GeometricObject.h"

class Triangle : public GeometricObject
{
	Vec3D a, b, c, n;
	void updateNormal()
	{
		n = (b - a) ^ (c - a);
	}
public:
	Triangle(Vec3D vertexA, Vec3D vertexB, Vec3D vertexC)
		: a(vertexA), b(vertexB), c(vertexC)
	{
		updateNormal();
	}
    virtual ~Triangle() {}
	void setA(Vec3D vertexA) { a = vertexA; updateNormal(); }
	void setB(Vec3D vertexB) { b = vertexB; updateNormal(); }
	void setC(Vec3D vertexC) { c = vertexC; updateNormal(); }
	Vec3D getA() { return a; }
	Vec3D getB() { return b; }
	Vec3D getC() { return c; }
	Vec3D getNormal() { return n.normalised(); }

	virtual HitRecord Hit(const Ray& ray)
	{
		HitRecord record;
		auto temp = ((a - ray.Origin) * n) / (ray.Direction * n);
		if (temp > kEpsilon)
		{
			auto p = ray.GetPoint(temp);
			auto denom = n * n;
			auto l1 = (n * ((b - a) ^ (p - a))) / denom;
			auto l2 = (n * ((c - b) ^ (p - b))) / denom;
			auto l3 = 1.0 - l1 - l2;
			if (l1 >= 0.0 && l1 <= 1.0 && l2 >= 0.0 && l2 <= 1.0 && l3 >= 0.0 && l3 <= 1.0)
			{
				record.Hit = true;
				record.Normal = n.normalised();
				record.HitPoint = p;
				record.T = temp;
			}
		}
		return record;
	}
};
