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
    virtual ~Light() {}
    virtual Vec3D GetDirection(const HitRecord& record) = 0;
    virtual RGBColor L(const HitRecord& record) = 0;
    virtual bool InShadow(const Ray& ray, const HitRecord& objs) = 0;
};

class Ambient : public Light
{
    float ls = 1.0;
    RGBColor color = Vec3D(1.0, 1.0, 1.0);
public:
	Ambient() = default;
    Ambient(float ls, RGBColor color) : ls(ls), color(color) {}
    virtual ~Ambient() {}
    inline Vec3D GetDirection(const HitRecord& record)
    {
        return Vec3D(0.0, 0.0, 0.0);
    }
    inline RGBColor L(const HitRecord& record)
    {
        return color * ls;
    }
    inline bool InShadow(const Ray& ray, const HitRecord& objs)
    {
        return false;
    }
};

class PointLight : public Light
{
    float ls;
    RGBColor color;
    Point3D location;
	float decay = 0.2;
public:
    PointLight(float ls, RGBColor color, Point3D location) : ls(ls), color(color), location(location) {}
	PointLight(float ls, RGBColor color, Point3D location, float decay) : ls(ls), color(color), location(location), decay(decay) {}
    virtual ~PointLight() {}
    inline Vec3D GetDirection(const HitRecord& record)
    {
        return (location - record.HitPoint).normalised();
    }
    inline RGBColor L(const HitRecord& record)
    {
        return color * ls / (decay * (location - record.HitPoint).lengthSquared() + 1);
    }
    bool InShadow(const Ray& ray, const HitRecord& record);
};

class ParallelLight : public Light
{
	float ls;
	RGBColor color;
	Vec3D direction;
public:
	ParallelLight(float ls, RGBColor color, Vec3D direction) : ls(ls), color(color), direction(direction) {}
	virtual ~ParallelLight() {}
	inline Vec3D GetDirection(const HitRecord& record)
	{
		return -direction.normalised();
	}
	inline RGBColor L(const HitRecord& record)
	{
		return color * ls;
	}
    bool InShadow(const Ray& ray, const HitRecord& record);
};
