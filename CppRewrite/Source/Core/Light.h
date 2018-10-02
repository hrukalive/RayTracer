/*
  ==============================================================================

    Light.h
    Created: 28 Sep 2018 11:03:21am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "Utility.h"

class Light
{
protected:
    bool Shadows;
public:
    virtual ~Light();
    virtual Vec3D GetDirection(const HitRecord& record) = 0;
    virtual RGBColor L(const HitRecord& record) = 0;
};

class Ambient : public Light
{
    float ls = 1.0;
    RGBColor color = Vec3D(1.0, 1.0, 1.0);
public:
    Ambient(float ls, RGBColor color) : ls(ls), color(color) {}
    virtual ~Ambient();
    inline Vec3D GetDirection(const HitRecord& record)
    {
        return Vec3D(0.0, 0.0, 0.0);
    }
    inline RGBColor L(const HitRecord& record)
    {
        return color * ls;
    }
};

class PointLight : public Light
{
    float ls;
    RGBColor color;
    Point3D location;
public:
    PointLight(float ls, RGBColor color, Point3D location) : ls(ls), color(color), location(location) {}
    virtual ~PointLight();
    inline Vec3D GetDirection(const HitRecord& record)
    {
        return (location - record.HitPoint).normalised();
    }
    inline RGBColor L(const HitRecord& record)
    {
        return color * ls;
    }
};
