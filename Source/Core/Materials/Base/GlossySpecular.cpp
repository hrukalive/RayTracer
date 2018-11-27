#include "BRDF.h"
#include "../../Globals.h"

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
    if (rdotwo > -0.703125)
    {
        L = cs * (ks * pow(rdotwo, exp));
    }
    return L;
} // From book chapter 15

RGBColor GlossySpecular::sampleF(const HitRecord& record, Vec3D& wi, const Vec3D& wo, FP_TYPE& pdf) const
{
    auto ndotwo = record.Normal * wo;
    auto r = -wo + (record.Normal * 2.0 * ndotwo);
    
    auto w = r.normalised();
    auto u = (Vec3D(0, 1, 0) ^ w).normalised();
    auto v = (w ^ u).normalised();
    if (abs(r.x) < 1e-7 && abs(r.z) < 1e-7)
    {
        if (r.y > 0)
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
    auto sample = sampler->SampleHemisphereSingle(exp);
    wi = u * sample.x + v * sample.y + w * sample.z;
    if (record.Normal * wi < 0.0)
        wi = -u * sample.x - v * sample.y + w * sample.z;
    wi = wi.normalised();
    FP_TYPE phongLobe = pow(r * wi, exp);
    pdf = phongLobe * (record.Normal * wi) + kEpsilon;
    return cs * ks * phongLobe;
}

RGBColor GlossySpecular::rho(const HitRecord& record, const Vec3D& wo) const
{
    return BLACK;
}

std::vector<std::pair<Vec3D, RGBColor>> GlossySpecular::GetWiAndF(const HitRecord& record, const Vec3D& wo)
{
    std::vector<std::pair<Vec3D, RGBColor>> ret;
    auto ndotwo = record.Normal * wo;
    auto r = -wo + (record.Normal * 2.0 * ndotwo);

    auto w = r.normalised();
    auto u = (Vec3D(0, 1, 0) ^ w).normalised();
    auto v = (w ^ u).normalised();
    if (abs(r.x) < 1e-7 && abs(r.z) < 1e-7)
    {
        if (r.y > 0)
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
    auto samples = sampler->SampleHemisphere(viewPlane->NumGlossySamples, exp);
    for (auto& sample : samples)
    {
        auto wi = u * sample.x + v * sample.y + w * sample.z;
        if (record.Normal * wi < 0.0)
            wi = -u * sample.x - v * sample.y + w * sample.z;
        wi = wi.normalised();
        FP_TYPE phongLobe = pow(r * wi, exp);
        FP_TYPE pdf = phongLobe * (record.Normal * wi) + kEpsilon;
        ret.push_back(std::make_pair(wi, cs * ks * phongLobe / pdf));
    }
    return ret;
}
