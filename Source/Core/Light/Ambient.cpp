#include "Light.h"
#include "../Globals.h"

Ambient::Ambient() : ls(1.0), color(RGBColor(1.0, 1.0, 1.0)) {}
Ambient::Ambient(const FP_TYPE ls, RGBColor color) : ls(ls), color(color) {}
inline Vec3D Ambient::GetDirection(const HitRecord& record)
{
    return Vec3D(0.0, 0.0, 0.0);
}
inline RGBColor Ambient::L(const HitRecord& record)
{
    return color * ls;
}
inline bool Ambient::InShadow(const Ray& ray, const HitRecord& objs)
{
    return false;
}
inline FP_TYPE Ambient::GetPower()
{
    return (color * ls).lengthSquared();
}
