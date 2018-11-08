/*
  ==============================================================================

    Tracer.cpp
    Created: 28 Sep 2018 11:04:29am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Tracer.h"

Tracer::Tracer(std::shared_ptr<World>& world) : worldPtr(world) {}
RGBColor Tracer::Trace(const Ray& ray) const
{
    return Trace(ray, 0);
}


RayCast::RayCast(std::shared_ptr<World>& world) : Tracer(world) {}
RGBColor RayCast::Trace(const Ray& ray, int depth) const
{
    if (depth > 0)
        return BLACK;
    HitRecord record = worldPtr->HitObjects(ray);
    return record.Hit ? (record.MaterialPtr->Shade(record)) : (worldPtr->GetBackgroundColor());
}


Whitted::Whitted(std::shared_ptr<World>& world) : Tracer(world), maxDepth(5) {}
Whitted::Whitted(std::shared_ptr<World>& world, int maxDepth) : Tracer(world), maxDepth(maxDepth) {}
RGBColor Whitted::Trace(const Ray& ray, int depth) const
{
    if (depth >= maxDepth)
        return BLACK;
    HitRecord record = worldPtr->HitObjects(ray);
    record.Depth = depth;
    return record.Hit ? (record.MaterialPtr->Shade(record)) : (worldPtr->GetBackgroundColor());
}

PathTrace::PathTrace(std::shared_ptr<World>& world) : Tracer(world), maxDepth(5) {}
PathTrace::PathTrace(std::shared_ptr<World>& world, int maxDepth) : Tracer(world), maxDepth(maxDepth) {}
RGBColor PathTrace::Trace(const Ray& ray, int depth) const
{
    if (depth >= maxDepth)
        return BLACK;
    HitRecord record = worldPtr->HitObjects(ray);
    record.Depth = depth;
    return record.Hit ? (record.MaterialPtr->PathShade(record)) : (worldPtr->GetBackgroundColor());
}
