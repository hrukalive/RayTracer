#include "BRDF.h"
#include "../../Globals.h"

Lambertian::Lambertian() : kd(0.0), cd(new ConstTexture()) {}

void Lambertian::SetKd(const FP_TYPE newKd) { kd = newKd; }
void Lambertian::SetCd(const RGBColor& newCd)
{
    cd.reset(new ConstTexture());
    std::dynamic_pointer_cast<ConstTexture>(cd)->setColor(newCd);
}
void Lambertian::SetCd(const std::shared_ptr<Texture> newCd)
{
    cd = newCd;
}

RGBColor Lambertian::f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
{
    return (cd->getColor(record) * kd) * INV_PI;
}

RGBColor Lambertian::sampleF(const HitRecord& record, Vec3D& wi, const Vec3D& wo, FP_TYPE& pdf) const
{
    auto w = record.Normal;
    auto v = (Vec3D(0, 1, 0) ^ w).normalised();
    auto u = (v ^ w).normalised();
    if (abs(w.x) < 1e-7 && abs(w.z) < 1e-7)
    {
        if (w.y > 0)
        {
            u = Vec3D(0.0, 0.0, 1.0);
            v = Vec3D(1.0, 0.0, 0.0);
            w = Vec3D(0.0, 1.0, 0.0);
        }
        else
        {
            u = Vec3D(1.0, 0.0, 0.0);
            v = Vec3D(0.0, 0.0, 1.0);
            w = Vec3D(0.0, -1.0, 0.0);
        }
    }
    
    auto sample = sampler->SampleHemisphereSingle(0.0);
    wi = (u * sample.x + v * sample.y + w * sample.z).normalised();
    pdf = record.Normal * wi * INV_PI;
    return cd->getColor(record) * kd * INV_PI;
}

RGBColor Lambertian::rho(const HitRecord& record, const Vec3D& wo) const
{
    return cd->getColor(record) * kd;
}
