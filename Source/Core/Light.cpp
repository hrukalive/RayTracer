/*
  ==============================================================================

    Light.cpp
    Created: 28 Sep 2018 11:03:21am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "../Globals.h"
#include "Light.h"
#include "World.h"
#include "Materials/Material.h"
#include "GeometricObject/GeometricObject.h"

bool PointLight::InShadow(const Ray& ray, const HitRecord& record)
{
    auto& objs = record.WorldPtr->GetObjects();
    auto d = (location - ray.Origin).length();
    for (int i = 0; i < objs.size(); i++)
    {
        auto tmp = objs[i]->Hit(ray);
        if (tmp.Hit && tmp.T < d)
        {
            return true;
        }
    }
    return false;
}

bool ParallelLight::InShadow(const Ray& ray, const HitRecord& record)
{
    auto& objs = record.WorldPtr->GetObjects();
    for (int i = 0; i < objs.size(); i++)
    {
        if (objs[i]->Hit(ray).Hit)
        {
            return true;
        }
    }
    return false;
}

AreaLight::AreaLight(std::shared_ptr<GeometricObject> obj, std::shared_ptr<Material> material)
    : ObjPtr(obj), MaterialPtr(material) {}

bool AreaLight::InShadow(const Ray& ray, const Point3D samplePoint, const HitRecord& record)
{
    auto& objs = record.WorldPtr->GetObjects();
    auto d = (samplePoint - ray.Origin) * ray.Direction.normalised();
    for (int i = 0; i < objs.size(); i++)
    {
        auto tmp = objs[i]->Hit(ray);
        if (tmp.Hit && tmp.T < d)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::pair<Point3D, std::pair<Vec3D, RGBColor>>> AreaLight::GetWiAndLGPDF(const HitRecord& record)
{
    std::vector<std::pair<Point3D, std::pair<Vec3D, RGBColor>>> ret;
    auto samples = ObjPtr->Sample();
    for (auto& sample : samples)
    {
        auto samplePoint = sample.first;
        auto lightNormal = sample.second;
        auto wi = (samplePoint - record.HitPoint).normalised();

        FP_TYPE ndotd = -lightNormal * wi;
        if (ndotd > 0.0)
        {
            auto L = MaterialPtr->GetLe(record);
            FP_TYPE d2 = decay * (samplePoint - record.HitPoint).lengthSquared() + 1;
            auto G = ndotd / d2;
            FP_TYPE pdf = 1.0 / viewPlane->NumAreaLightSamples;
            ret.push_back(std::make_pair(samplePoint, std::make_pair(wi, L * G * pdf)));
        }
        else
        {
            ret.push_back(std::make_pair(samplePoint, std::make_pair(wi, BLACK)));
        }
    }
    return ret;
}

Vec3D AreaLight::GetDirection(const HitRecord& record)
{
    return ZERO;
}

RGBColor AreaLight::L(const HitRecord& record)
{
    return BLACK;
}
