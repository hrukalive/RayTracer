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
	Point3D Center;
	FP_TYPE Radius = 1.0f;
public:
	Sphere(Point3D center, FP_TYPE radius) : Center(center), Radius(radius) {}
    virtual ~Sphere() {}
	void setCenter(Point3D center) { Center = center; }
	void setRadius(FP_TYPE radius) { Radius = radius; }
	Point3D getCenter() { return Center; }
	FP_TYPE getRadius() { return Radius; }

	virtual HitRecord Hit(const Ray& ray)
	{
		HitRecord record;
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
				record.Hit = true;
				record.Normal = (p - Center).normalised();
				record.HitPoint = p;
				record.T = t1;
			}
			else if (t2 > kEpsilon) 
			{
				auto p = ray.GetPoint(t2);
				record.Hit = true;
				record.Normal = (p - Center).normalised();
				record.HitPoint = p;
				record.T = t2;
			}
		}

		return record;
	}
};
