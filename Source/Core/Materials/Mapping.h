/*
  ==============================================================================

    Mapping.h
    Created: 13 Nov 2018 10:11:46am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../Utility/Utility.h"

class Mapping
{
public:
    virtual ~Mapping() = default;
    virtual Point2D getTexelCoord(const Point3D hitPoint, const int hres, const int vres) const = 0;
};

class SphericalMap : public Mapping
{
public:
    virtual ~SphericalMap() = default;
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

class LightProbe : public Mapping
{
    bool isPanoramic = false;
public:
    virtual ~LightProbe() = default;
    void setPanoramic(const bool panoramic)
    {
        isPanoramic = panoramic;
    }
    Point2D getTexelCoord(const Point3D hitPoint, const int hres, const int vres) const override
    {
        FP_TYPE x = hitPoint.x, y = hitPoint.y, z = hitPoint.z;
        FP_TYPE d = sqrt(x * x + y * y);
        FP_TYPE sinBeta = y / d;
        FP_TYPE cosBeta = x / d;
        FP_TYPE alpha = isPanoramic ? acos(-z) : acos(z);
        FP_TYPE r = alpha * INV_PI;
        FP_TYPE u = (1 + r * cosBeta) * 0.5;
        FP_TYPE v = (1 + r * sinBeta) * 0.5;
        return Point2D((hres - 1) * u, (vres - 1) * v);
    }
};
