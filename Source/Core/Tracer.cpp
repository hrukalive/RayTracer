/*
  ==============================================================================

    Tracer.cpp
    Created: 28 Sep 2018 11:04:29am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Tracer.h"
#include "Globals.h"

std::pair<bool, RGBColor> Tracer::Trace(const Ray& ray) const
{
    return Trace(ray, 0);
}


RayCast::RayCast() : Tracer() {}
std::pair<bool, RGBColor> RayCast::Trace(const Ray& ray, int depth) const
{
    if (depth > 0)
        return std::make_pair(true, BLACK);
    HitRecord record = world->HitObjects(ray);
    return std::make_pair(true, (record.Hit ? (record.MaterialPtr->Shade(record)) : (world->GetBackgroundColor())));
}


Whitted::Whitted() : Tracer(), maxDepth(5) {}
Whitted::Whitted(int maxDepth) : Tracer(), maxDepth(maxDepth) {}
std::pair<bool, RGBColor> Whitted::Trace(const Ray& ray, int depth) const
{
    if (depth >= maxDepth)
        return std::make_pair(true, BLACK);
    HitRecord record = world->HitObjects(ray);
    record.Depth = depth;
    return std::make_pair(true, (record.Hit ? (record.MaterialPtr->Shade(record)) : (world->GetBackgroundColor())));
}

PathTrace::PathTrace() : Tracer(), maxDepth(5) {}
PathTrace::PathTrace(int maxDepth) : Tracer(), maxDepth(maxDepth) {}
std::pair<bool, RGBColor> PathTrace::Trace(const Ray& ray, int depth) const
{
    if (depth >= maxDepth)
        return std::make_pair(false, BLACK);
    HitRecord record = world->HitObjects(ray);
    record.Depth = depth;
    if (!record.Hit)
        return std::make_pair(true, BLACK);
    else
    {
        auto shadeResult = record.MaterialPtr->PathShade(record);
        if (dynamic_cast<const Emissive*>(record.MaterialPtr.get()) != nullptr)
            shadeResult.second = std::dynamic_pointer_cast<Emissive>(record.MaterialPtr)->GetLe(record).normalised();
        return std::make_pair(shadeResult.first, shadeResult.second);
    }
}

PhotonMapPreTrace::PhotonMapPreTrace() : Tracer(), maxDepth(5) {}
PhotonMapPreTrace::PhotonMapPreTrace(int maxDepth) : Tracer(), maxDepth(maxDepth) {}
std::pair<bool, RGBColor> PhotonMapPreTrace::Trace(const Ray& ray, int depth) const
{
    if (depth >= maxDepth)
        return std::make_pair(false, BLACK);
    HitRecord record = world->HitObjects(ray);
    record.Depth = depth;
    if (record.Hit)
        record.MaterialPtr->PhotonMapping(record);
    return std::make_pair(false, BLACK);
}

PhotonMapTrace::PhotonMapTrace() : Tracer(), maxDepth(5) {}
PhotonMapTrace::PhotonMapTrace(int maxDepth) : Tracer(), maxDepth(maxDepth) {}
std::pair<bool, RGBColor> PhotonMapTrace::Trace(const Ray& ray, int depth) const
{
    if (depth >= maxDepth)
        return std::make_pair(false, BLACK);
    HitRecord record = world->HitObjects(ray);
    record.Depth = depth;
    if (!record.Hit)
        return std::make_pair(true, BLACK);
    else
        return std::make_pair(true, record.MaterialPtr->PhotonShade(record));
}
