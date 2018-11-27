#include "Tracer.h"
#include "../Globals.h"

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
