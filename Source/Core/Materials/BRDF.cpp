#include "BRDF.h"

Lambertian::Lambertian() : kd(0.0), cd(BLACK) {}
Lambertian::Lambertian(const FP_TYPE kd, const RGBColor& cd) : kd(kd), cd(cd) {}

void Lambertian::SetKd(const FP_TYPE newKd) { kd = newKd; }
void Lambertian::SetCd(const RGBColor& newCd) { cd = newCd; }

RGBColor Lambertian::f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
{
    return (cd * kd) * INV_PI;
}

RGBColor Lambertian::sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
{
    return BLACK;
}

RGBColor Lambertian::rho(const HitRecord& record, const Vec3D& wo) const
{
    return cd * kd;
}

PerfectSpecular::PerfectSpecular() : kr(0.0), cr(BLACK) {}
PerfectSpecular::PerfectSpecular(const FP_TYPE kr, const RGBColor& cr) : kr(kr), cr(cr) {}

void PerfectSpecular::SetKr(const FP_TYPE newKr) { kr = newKr; }
void PerfectSpecular::SetCr(const RGBColor& newCr) { cr = newCr; }

RGBColor PerfectSpecular::f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
{
    return BLACK;
}

RGBColor PerfectSpecular::sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
{
    FP_TYPE ndotwo = record.Normal * wo;
    Vec3D r = -wo + record.Normal * 2.0 * ndotwo;
    return cr * kr / (record.Normal * r);
}

RGBColor PerfectSpecular::rho(const HitRecord& record, const Vec3D& wo) const
{
    return BLACK;
}

GlossySpecular::GlossySpecular() : ks(1.0), exp(10.0), cs(WHITE) {}
GlossySpecular::GlossySpecular(const FP_TYPE ks, const FP_TYPE exp, const RGBColor& cs) : ks(ks), exp(exp), cs(cs) {}

void GlossySpecular::SetKs(const FP_TYPE newKs) { ks = newKs; }
void GlossySpecular::SetCs(const RGBColor& newCs) { cs = newCs; }
void GlossySpecular::SetE(const FP_TYPE newExp) { exp = newExp; }

RGBColor GlossySpecular::f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
{
    RGBColor L;
    auto ndotwi = record.Normal * wi;
    auto r = -wi + (record.Normal * 2.0 * ndotwi);
    auto rdotwo = r * wo;
    if (rdotwo > 0.0)
    {
        L = cs * (ks * pow(rdotwo, exp));
    }
    return L;
} // From book chapter 15

RGBColor GlossySpecular::sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
{
    return BLACK;
}

RGBColor GlossySpecular::rho(const HitRecord& record, const Vec3D& wo) const
{
    return BLACK;
}
