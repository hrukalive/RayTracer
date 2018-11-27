#include "Material.h"

RGBColor NormalShade::Shade(const HitRecord& record)
{
    return (record.Normal + Vec3D(1.0, 1.0, 1.0)) / 2.0;
}
std::pair<bool, RGBColor> NormalShade::PathShade(const HitRecord& record)
{
    return std::make_pair(true, Shade(record));
}
RGBColor NormalShade::PhotonShade(const HitRecord& record)
{
    return (record.Normal + Vec3D(1.0, 1.0, 1.0)) / 2.0;
}
