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
	Vector3D<FP_TYPE> a, b, c, n;
	void updateNormal()
	{
		n = (b - a) ^ (c - a);
	}
public:
	Triangle(Vector3D<FP_TYPE> vertexA, Vector3D<FP_TYPE> vertexB, Vector3D<FP_TYPE> vertexC)
		: a(vertexA), b(vertexB), c(vertexC)
	{
		updateNormal();
	}
	void setA(Vector3D<FP_TYPE> vertexA) { a = vertexA; updateNormal(); }
	void setB(Vector3D<FP_TYPE> vertexB) { b = vertexB; updateNormal(); }
	void setC(Vector3D<FP_TYPE> vertexC) { c = vertexC; updateNormal(); }
	Vector3D<FP_TYPE> getA() { return a; }
	Vector3D<FP_TYPE> getB() { return b; }
	Vector3D<FP_TYPE> getC() { return c; }
	Vector3D<FP_TYPE> getNormal() { return n.normalised(); }

	virtual bool Hit(Ray& ray, HitRecord& record)
	{
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
				record.Normal = n.normalised();
				record.Position = p;
				record.T = temp;
				return true;
			}
		}
		return false;
	}
};
