#include "Material.h"

RGBColor Emissive::Shade(const HitRecord& record)
{
    if (-record.Normal * record.Ray.Direction > 0.0 && !record.NormalFlipped)
        return ce * std::min((FP_TYPE)1.0, ls);
    else
        return BLACK;
}

std::pair<bool, RGBColor> Emissive::PathShade(const HitRecord& record)
{
    if (-record.Normal * record.Ray.Direction > 0.0 && !record.NormalFlipped)
        return std::make_pair(true, ce * ls);
    else
        return std::make_pair(true, BLACK);
}

RGBColor Emissive::PhotonShade(const HitRecord& record)
{
    if (-record.Normal * record.Ray.Direction > 0.0 && !record.NormalFlipped)
        return ce * std::min((FP_TYPE)1.0, ls);
    else
        return BLACK;
}
