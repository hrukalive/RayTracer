/*
  ==============================================================================

    Light.cpp
    Created: 28 Sep 2018 11:03:21am
    Author:  Bowen Sun

  ==============================================================================
*/

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

//AreaLight::AreaLight(std::shared_ptr<GeometricObject> obj, std::shared_ptr<Material> material)
//    : ObjPtr(obj), MaterialPtr(material) {}
//
//bool AreaLight::InShadow(const Ray& ray, const HitRecord& record)
//{
//    auto& objs = record.WorldPtr->GetObjects();
//    auto d = (samplePoint - ray.Origin) * ray.Direction.normalised();
//    for (int i = 0; i < objs.size(); i++)
//    {
//        auto tmp = objs[i]->Hit(ray);
//        if (tmp.Hit && tmp.T < d)
//        {
//            return true;
//        }
//    }
//    return false;
//}
//
//Vec3D AreaLight::GetDirection(const HitRecord& record)
//{
//    BBox& bbox = ObjPtr->Sample();
//    lightNormal = ObjPtr->GetNormal(samplePoint);
//    wi = (samplePoint - record.HitPoint).normalised();
//    return wi;
//}
//
//RGBColor AreaLight::L(const HitRecord& record)
//{
//    FP_TYPE ndotd = -lightNormal * wi;
//    if (ndotd > 0.0)
//        return MaterialPtr->GetLe(record);
//    else
//        return BLACK;
//}
//
//FP_TYPE AreaLight::G(const HitRecord& record)
//{
//    FP_TYPE ndotd = -lightNormal * wi;
//    FP_TYPE d2 = (samplePoint - record.HitPoint).lengthSquared();
//    return ndotd / d2;
//}
//FP_TYPE AreaLight::V(const HitRecord& record)
//{
//    return ObjPtr->pdf(record);
//}
