/*
  ==============================================================================

    Mapping.h
    Created: 13 Nov 2018 10:11:46am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../../Utility/Utility.h"

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
    Point2D getTexelCoord(const Point3D hitPoint, const int hres, const int vres) const override;
};

class LightProbe : public Mapping
{
    bool isPanoramic = false;
public:
    virtual ~LightProbe() = default;
    void setPanoramic(const bool panoramic);
    Point2D getTexelCoord(const Point3D hitPoint, const int hres, const int vres) const override;
};
