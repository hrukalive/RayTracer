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
    void UpdateBoundingBox() override;
public:
    Sphere(Point3D center, FP_TYPE radius);
    virtual ~Sphere() = default;
    void setCenter(Point3D center);
    void setRadius(FP_TYPE radius);
    Point3D getCenter();
    FP_TYPE getRadius();

    virtual HitRecord Hit(const Ray& ray) override;
};
