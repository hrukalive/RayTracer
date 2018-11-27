#include "BRDF.h"
#include "../../Globals.h"

PerfectSpecular::PerfectSpecular() : kr(0.0), cr(BLACK) {}
PerfectSpecular::PerfectSpecular(const FP_TYPE kr, const RGBColor& cr) : kr(kr), cr(cr) {}

void PerfectSpecular::SetKr(const FP_TYPE newKr) { kr = newKr; }
void PerfectSpecular::SetCr(const RGBColor& newCr) { cr = newCr; }

RGBColor PerfectSpecular::f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
{
    return BLACK;
}

RGBColor PerfectSpecular::sampleF(const HitRecord& record, Vec3D& wi, const Vec3D& wo, FP_TYPE& pdf) const
{
    FP_TYPE ndotwo = record.Normal * wo;
    wi = -wo + record.Normal * 2.0 * ndotwo;
    pdf = record.Normal * wi;
    return cr * kr;
}

RGBColor PerfectSpecular::rho(const HitRecord& record, const Vec3D& wo) const
{
    return BLACK;
}
