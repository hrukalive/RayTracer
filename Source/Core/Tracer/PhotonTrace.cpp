#include "Tracer.h"
#include "../Globals.h"

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
