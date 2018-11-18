/*
  ==============================================================================

    IObject.h
    Created: 5 Oct 2018 7:38:12pm
    Author:  bowen

  ==============================================================================
*/

#pragma once
#include "../Utility.h"

class IObject
{
public:
    virtual ~IObject() = default;
	virtual HitRecord Hit(const Ray& ray) = 0;
    std::vector<HitRecord> getAllHit(const Ray& ray)
    {
        std::vector<HitRecord> ret;
        Ray tmpRay = ray;
        HitRecord record = Hit(tmpRay);
        while (record.Hit)
        {
            ret.push_back(record);
            tmpRay = Ray(tmpRay.GetPoint(record.T + kEpsilon), ray.Direction);
            record = Hit(tmpRay);
        }
        return ret;
    }
};
