/*
  ==============================================================================

    Sphere.h
    Created: 28 Sep 2018 11:03:05am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"

class Sphere : public GeometricObject
{
	Point3D Center;
	FP_TYPE Radius = 1.0f;
protected:
	void UpdateBoundingBox() override
	{
		Point3D delta(Radius, Radius, Radius);
		boundingBox = BBox(Center - delta, Center + delta);
	}
public:
	Sphere(Point3D center, FP_TYPE radius) : Center(center), Radius(radius) { UpdateBoundingBox(); }
    virtual ~Sphere() {}
	void setCenter(Point3D center) { Center = center; UpdateBoundingBox(); }
	void setRadius(FP_TYPE radius) { Radius = radius; UpdateBoundingBox(); }
	Point3D getCenter() { return Center; }
	FP_TYPE getRadius() { return Radius; }

	virtual HitRecord Hit(const Ray& ray) override
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
                auto n = p - Center;
				record.Hit = true;
				record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
				record.HitPoint = p;
                record.MaterialPtr = materialPtr;
                record.Ray = ray;
				record.T = t1;
			}
			else if (t2 > kEpsilon) 
			{
				auto p = ray.GetPoint(t2);
                auto n = p - Center;
				record.Hit = true;
				record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
				record.HitPoint = p;
                record.MaterialPtr = materialPtr;
                record.Ray = ray;
				record.T = t2;
			}
		}
		return record;
	}
};
