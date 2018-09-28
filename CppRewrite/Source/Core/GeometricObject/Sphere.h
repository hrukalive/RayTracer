/*
  ==============================================================================

    Sphere.h
    Created: 28 Sep 2018 11:03:05am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../Utility.h"
#include "GeometricObject.h"

class Sphere : public GeometricObject
{
	Vector3D<FP_TYPE> Center;
	FP_TYPE Radius = 1.0f;
public:
	Sphere(Vector3D<FP_TYPE> center, FP_TYPE radius) : Center(center), Radius(radius) {}
	void setCenter(Vector3D<FP_TYPE> center) { Center = center; }
	void setRadius(FP_TYPE radius) { Radius = radius; }
	Vector3D<FP_TYPE> getCenter() { return Center; }
	FP_TYPE getRadius() { return Radius; }

	virtual bool Hit(Ray& ray, HitRecord& record)
	{
		auto temp = ray.Origin - Center;
		auto a = ray.Direction * ray.Direction;
		auto b = 2.0 * (temp * ray.Direction);
		auto c = temp * temp - Radius * Radius;
		auto disc = b * b - 4.0 * a * c;

		if (disc >= 0.0)
		{
			auto t1 = (-b - sqrt(disc)) / a * 0.5;
			auto t2 = (-b + sqrt(disc)) / a * 0.5;

			if (t1 > kEpsilon)
			{
				auto p = ray.GetPoint(t1);
				record.Normal = (p - Center).normalised();
				record.Position = p;
				record.T = t1;
				return true;
			}
			else if (t2 > kEpsilon) 
			{
				auto p = ray.GetPoint(t2);
				record.Normal = (p - Center).normalised();
				record.Position = p;
				record.T = t2;
				return true;
			}
		}

		return false;
	}
};
