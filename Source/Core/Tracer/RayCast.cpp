#include "Tracer.h"
#include "../Globals.h"

RayCast::RayCast() : Tracer() {}
std::pair<bool, RGBColor> RayCast::Trace(const Ray& ray, int depth) const
{
    if (depth > 0)
        return std::make_pair(true, BLACK);
    HitRecord record = world->HitObjects(ray);
    return std::make_pair(true, (record.Hit ? (record.MaterialPtr->Shade(record)) : (world->GetBackgroundColor())));
}
