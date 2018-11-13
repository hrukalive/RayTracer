/*
  ==============================================================================

    Mapping.h
    Created: 13 Nov 2018 10:11:46am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../Utility.h"

class Mapping
{
public:
    virtual ~Mapping() = default;
    virtual Point2D getTexelCoord(const Point3D hitPoint, const int hres, const int vres) const = 0;
};

class SphericalMap : public Mapping
{
public:
    Point2D getTexelCoord(const Point3D hitPoint, const int hres, const int vres) const override
    {
        FP_TYPE theta = acos(hitPoint.y);
        FP_TYPE phi = atan2(hitPoint.x, hitPoint.z);
        if (phi < 0.0)
            phi += TWO_PI;
        FP_TYPE u = phi * INV_TWO_PI;
        FP_TYPE v = 1 - theta * INV_PI;
        return Point2D((hres - 1) * u, (vres - 1) * v);
    }
};
