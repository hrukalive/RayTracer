/*
  ==============================================================================

    GeometricObject.cpp
    Created: 18 Nov 2018 4:20:09pm
    Author:  bowen

  ==============================================================================
*/

#include "GeometricObject.h"
#include "../Globals.h"

std::vector<HitRecord> GeometricObject::getHitInterval(const Ray& ray)
{
    BBox box = world->GetBoundingBox();
    FP_TYPE dist = std::max((ray.Origin - box.GetMinPoint()).length(), (ray.Origin - box.GetMaxPoint()).length());
    dist += (box.GetMaxPoint() - box.GetMinPoint()).length();

    std::vector<HitRecord> ret;
    FP_TYPE tmpT = -dist;
    Ray tmpRay = ray;
    FP_TYPE acc = -dist;
    while (true)
    {
        tmpRay = Ray(tmpRay.GetPoint(tmpT + kEpsilon), ray.Direction);
        HitRecord record = Hit(tmpRay);
        if (!record.Hit)
            return ret;
        tmpT = record.T;
        record.T += acc;
        acc = record.T;
        ret.push_back(record);
        tmpRay = Ray(tmpRay.GetPoint(tmpT + kEpsilon), ray.Direction);
        record = Hit(tmpRay);
        if (!record.Hit)
        {
            ret.push_back(ret[ret.size() - 1]);
            return ret;
        }
        else
        {
            tmpT = record.T;
            record.T += acc;
            acc = record.T;
            ret.push_back(record);
        }
    }
    return ret;
}
