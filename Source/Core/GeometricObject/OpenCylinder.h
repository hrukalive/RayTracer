/*
  ==============================================================================

    OpenCylinder.h
    Created: 17 Nov 2018 10:01:55pm
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "GeometricObject.h"

class OpenCylinder : public GeometricObject
{
    FP_TYPE h, r;
    Point3D bc;
protected:
    void UpdateBoundingBox() override;
public:
    OpenCylinder(Point3D bottomCenter, FP_TYPE radius, FP_TYPE h);
    virtual ~OpenCylinder() = default;
    virtual HitRecord Hit(const Ray& ray) override;
};
