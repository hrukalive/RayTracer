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
    FP_TYPE ls = 1.0;
    RGBColor color = Vec3D(1.0, 1.0, 1.0);
public:
	Ambient() = default;
    Ambient(const FP_TYPE ls, RGBColor color) : ls(ls), color(color) {}
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
    FP_TYPE ls;
    RGBColor color;
    Point3D location;
    FP_TYPE decay = 0.2;
public:
    PointLight(FP_TYPE ls, RGBColor color, Point3D location) : ls(ls), color(color), location(location) {}
	PointLight(FP_TYPE ls, RGBColor color, Point3D location, FP_TYPE decay) : ls(ls), color(color), location(location), decay(decay) {}
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
    FP_TYPE ls;
	RGBColor color;
	Vec3D direction;
public:
	ParallelLight(FP_TYPE ls, RGBColor color, Vec3D direction) : ls(ls), color(color), direction(direction) {}
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

class AreaLight : public Light
{
    std::shared_ptr<GeometricObject> ObjPtr;
    std::shared_ptr<Emissive> MaterialPtr;
    Point3D samplePoint;
    Vec3D lightNormal;
    Vec3D wi;
public:
    AreaLight(std::shared_ptr<GeometricObject> obj, std::shared_ptr<Material> material) : ObjPtr(obj), MaterialPtr(material) {}
    virtual ~AreaLight() {}
    inline Vec3D GetDirection(const HitRecord& record)
    {
        samplePoint = ObjPtr->Sample();
        lightNormal = ObjPtr->GetNormal(samplePoint);
        wi = (samplePoint - record.HitPoint).normalised();
        return wi;
    }
    inline RGBColor L(const HitRecord& record)
    {
        FP_TYPE ndotd = -lightNormal * wi;
        if (ndotd > 0.0)
            return MaterialPtr->GetLe(record);
        else
            return BLACK;
    }
    inline FP_TYPE G(const HitRecord& record)
    {
        FP_TYPE ndotd = -lightNormal * wi;
        FP_TYPE d2 = (samplePoint - record.HitPoint).lengthSquared();
        return ndotd / d2;
    }
    inline FP_TYPE pdf(const HitRecord& record)
    {
        return ObjPtr->pdf(record);
    }
    bool InShadow(const Ray& ray, const HitRecord& record);
};