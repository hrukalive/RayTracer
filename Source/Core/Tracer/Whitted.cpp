#include "Tracer.h"
#include "../Globals.h"

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
