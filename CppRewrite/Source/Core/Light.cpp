/*
  ==============================================================================

    Light.cpp
    Created: 28 Sep 2018 11:03:21am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Light.h"
#include "World.h"

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
