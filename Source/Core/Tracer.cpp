/*
  ==============================================================================

    Tracer.cpp
    Created: 28 Sep 2018 11:04:29am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Tracer.h"

Tracer::Tracer(std::shared_ptr<World>& world) : worldPtr(world) {}
std::pair<bool, RGBColor> Tracer::Trace(const Ray& ray) const
{
    return Trace(ray, 0);
}


RayCast::RayCast(std::shared_ptr<World>& world) : Tracer(world) {}
std::pair<bool, RGBColor> RayCast::Trace(const Ray& ray, int depth) const
{
    if (depth > 0)
        return std::make_pair(true, BLACK);
    HitRecord record = worldPtr->HitObjects(ray);
    return std::make_pair(true, (record.Hit ? (record.MaterialPtr->Shade(record)) : (worldPtr->GetBackgroundColor())));
}


Whitted::Whitted(std::shared_ptr<World>& world) : Tracer(world), maxDepth(5) {}
Whitted::Whitted(std::shared_ptr<World>& world, int maxDepth) : Tracer(world), maxDepth(maxDepth) {}
std::pair<bool, RGBColor> Whitted::Trace(const Ray& ray, int depth) const
{
    if (depth >= maxDepth)
        return std::make_pair(true, BLACK);
    HitRecord record = worldPtr->HitObjects(ray);
    record.Depth = depth;
    return std::make_pair(true, (record.Hit ? (record.MaterialPtr->Shade(record)) : (worldPtr->GetBackgroundColor())));
}

PathTrace::PathTrace(std::shared_ptr<World>& world) : Tracer(world), maxDepth(5) {}
PathTrace::PathTrace(std::shared_ptr<World>& world, int maxDepth) : Tracer(world), maxDepth(maxDepth) {}
std::pair<bool, RGBColor> PathTrace::Trace(const Ray& ray, int depth) const
{
    if (depth >= maxDepth)
        return std::make_pair(false, BLACK);
    HitRecord record = worldPtr->HitObjects(ray);
    record.Depth = depth;
    if (!record.Hit)
        return std::make_pair(true, BLACK);
    else
    {
        auto shadeResult = record.MaterialPtr->PathShade(record);
        if (dynamic_cast<const Emissive*>(record.MaterialPtr.get()) != nullptr)
            shadeResult.second = std::dynamic_pointer_cast<Emissive>(record.MaterialPtr)->GetLe(record);
        return std::make_pair(shadeResult.first, shadeResult.second);
    }
}
