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


RGBColor RayCast::Trace(const Ray& ray, int depth) const
{
    HitRecord record = worldPtr->HitObjects(ray);
    record.WorldPtr = worldPtr;
    return record.Hit ? (record.MaterialPtr->Shade(record)) : (worldPtr->GetBackgroundColor());
}
RayCast::RayCast(std::shared_ptr<World>& world) : Tracer(world) {}
