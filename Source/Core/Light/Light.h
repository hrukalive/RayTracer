/*
  ==============================================================================

    Light.h
    Created: 28 Sep 2018 11:03:21am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "../Utility/Utility.h"

class Light
{
protected:
    bool Shadows;
public:
    virtual ~Light() = default;
    virtual Vec3D GetDirection(const HitRecord& record) = 0;
    virtual RGBColor L(const HitRecord& record) = 0;
    virtual bool InShadow(const Ray& ray, const HitRecord& objs) = 0;
    virtual std::vector<Ray> EmitPhoton() { return std::vector<Ray>(); }
    virtual FP_TYPE GetPower() = 0;

    static std::shared_ptr<Light> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};

class Ambient : public Light
{
    FP_TYPE ls;
    RGBColor color;
public:
    Ambient();
    Ambient(const FP_TYPE ls, RGBColor color);
    virtual ~Ambient() = default;
    Vec3D GetDirection(const HitRecord& record) override;
    RGBColor L(const HitRecord& record) override;
    bool InShadow(const Ray& ray, const HitRecord& objs) override;
    FP_TYPE GetPower() override;

    static std::shared_ptr<Light> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
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
    virtual ~PointLight() = default;
    inline void setLs(FP_TYPE newLs)
    {
        ls = newLs;
    }
    inline Vec3D GetDirection(const HitRecord& record) override
    {
        return (location - record.HitPoint).normalised();
    }
    inline RGBColor L(const HitRecord& record) override
    {
        return color * ls / (decay * (location - record.HitPoint).lengthSquared() + 1);
    }
    inline FP_TYPE GetPower() override
    {
        return (color * ls).lengthSquared();
    }
    std::vector<Ray> EmitPhoton() override;
    bool InShadow(const Ray& ray, const HitRecord& record) override;

    static std::shared_ptr<Light> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};

class ParallelLight : public Light
{
    FP_TYPE ls;
	RGBColor color;
	Vec3D direction;
public:
	ParallelLight(FP_TYPE ls, RGBColor color, Vec3D direction) : ls(ls), color(color), direction(direction) {}
    virtual ~ParallelLight() = default;
	inline Vec3D GetDirection(const HitRecord& record) override
	{
		return -direction.normalised();
	}
	inline RGBColor L(const HitRecord& record) override
	{
		return color * ls;
	}
    inline FP_TYPE GetPower() override
    {
        return (color * ls).lengthSquared();
    }
    bool InShadow(const Ray& ray, const HitRecord& record) override;

    static std::shared_ptr<Light> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};

class Emissive;
class GeometricObject;
class AreaLight : public Light
{
    std::shared_ptr<GeometricObject> ObjPtr;
    std::shared_ptr<Material> MaterialPtr;
    Vec3D wi, lightNormal;
    Point3D samplePoint;
    FP_TYPE decay = 0.2;
public:
    AreaLight(std::shared_ptr<GeometricObject> obj, std::shared_ptr<Material> material);
    virtual ~AreaLight() = default;
    void setLs(FP_TYPE ls);
    Vec3D GetDirection(const HitRecord& record) override;
    RGBColor L(const HitRecord& record) override;
    FP_TYPE G(const HitRecord& record) const;
    FP_TYPE pdf(const HitRecord& record) const;
    bool InShadow(const Ray& ray, const HitRecord& record) override;
    bool InShadow(const Ray& ray, const Point3D samplePoint, const HitRecord& record);

    std::vector<Ray> EmitPhoton() override;
    FP_TYPE GetPower() override;

    std::vector<std::pair<Point3D, std::pair<Vec3D, RGBColor>>> GetWiAndLGPDF(const HitRecord& record);

    static std::shared_ptr<Light> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};
