/*
  ==============================================================================

    Box.h
    Created: 5 Nov 2018 2:25:16pm
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "../GeometricObject.h"

class Box : public GeometricObject
{
    Point3D boxMin, boxMax;
protected:
    void UpdateBoundingBox() override;
public:
    Box(const Point3D min, const Point3D max);
    virtual ~Box() = default;
    Point3D GetMinPoint();
    Point3D GetMaxPoint();

    HitRecord Hit(const Ray& ray) override;
};