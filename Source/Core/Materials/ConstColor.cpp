#include "Material.h"

ConstColor::ConstColor(RGBColor color) : c(color) {}
RGBColor ConstColor::Shade(const HitRecord& record)
{
    return c;
}
std::pair<bool, RGBColor> ConstColor::PathShade(const HitRecord& record)
{
    return std::make_pair(true, c);
}
RGBColor ConstColor::PhotonShade(const HitRecord& record)
{
    return c;
}
